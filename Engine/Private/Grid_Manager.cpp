#include "EnginePCH.h"
#include "Grid_Manager.h"
#include "Grid.h"
#include "GameObject.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Model.h"
#include "Mesh.h"

HRESULT CGrid_Manager::Reserve_Manager(const SCENE _eSceneMax, _float3 _vGridSize)
{
	m_arrGridLayers	= Function::CreateDynamicArray<GridLayers>(IDX(_eSceneMax), false);

	m_vGridSize	= _vGridSize;

	m_eSceneMax	= _eSceneMax;

	return S_OK;
}

HRESULT CGrid_Manager::Register_VIBuffer(const SCENE _eScene, const wstring& _strGridLayerTag, shared_ptr<CGameObject> _pGameObject)
{
	HRESULT hr = S_OK;

	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CGrid_Manager::Register_VIBuffer", "Invalid Range: SCENE");
	}

	if (nullptr == _pGameObject)
	{
		MSG_RETURN(E_FAIL, "CGrid_Manager::Register_VIBuffer", "Nullptr Exception");
	}

	shared_ptr<CTransform> pTransform = _pGameObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == pTransform)
	{
		MSG_RETURN(E_FAIL, "CGrid_Manager::Register_VIBuffer", "Nullptr Exception: CTransform");
	}

	if (shared_ptr<CVIBuffer> pVIBuffer = _pGameObject->Get_Component<CVIBuffer>(COMPONENT::VIBUFFER))
	{
		if (FAILED(Register_VIBuffer(_eScene, _strGridLayerTag, pVIBuffer, pTransform)))
		{
			MSG_RETURN(E_FAIL, "CGrid_Manager::Register_VIBuffer", "Failed to Register_VIBuffer");
		}
	}
	else if (shared_ptr<CModel> pModel = _pGameObject->Get_Component<CModel>(COMPONENT::MODEL))
	{
		pModel->Iterate_Meshes(
			[&](shared_ptr<CMesh> _pMesh)
			{
				if (FAILED(Register_VIBuffer(_eScene, _strGridLayerTag, _pMesh, pTransform)))
				{
					hr = E_FAIL;
				}

				return false;
			}
		);
	}

	return hr;
}

HRESULT CGrid_Manager::Register_VIBuffer(const SCENE _eScene, const wstring& _strGridLayerTag, shared_ptr<class CVIBuffer> _pVIBuffer, shared_ptr<class CTransform> _pTransform)
{
	if (nullptr == _pTransform)
	{
		MSG_RETURN(E_FAIL, "CGrid_Manager::Register_VIBuffer", "Nullptr Exception: CTransform");
	}
	if (nullptr == _pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CGrid_Manager::Register_VIBuffer", "Nullptr Exception: CVIBuffer");
	}

	_pVIBuffer->Iterate_Indices(
		[&](_uint _i0, _uint _i1, _uint _i2)->_bool
		{
			pair<_float3*, _uint> pairVertices = _pVIBuffer->Get_Vertices();

			_float3 arrVertex[3] =
			{
				 XMVector3TransformCoord(pairVertices.first[_i0], _pTransform->Get_Matrix()),
				 XMVector3TransformCoord(pairVertices.first[_i1], _pTransform->Get_Matrix()),
				 XMVector3TransformCoord(pairVertices.first[_i2], _pTransform->Get_Matrix())
			};

			_float3 vMinBound = _float3(
				Function::Min(arrVertex[0].x, arrVertex[1].x, arrVertex[2].x),
				Function::Min(arrVertex[0].y, arrVertex[1].y, arrVertex[2].y),
				Function::Min(arrVertex[0].z, arrVertex[1].z, arrVertex[2].z)
			);

			_float3 vMaxBound = _float3(
				Function::Max(arrVertex[0].x, arrVertex[1].x, arrVertex[2].x),
				Function::Max(arrVertex[0].y, arrVertex[1].y, arrVertex[2].y),
				Function::Max(arrVertex[0].z, arrVertex[1].z, arrVertex[2].z)
			);

			_float3 vMinKey = _float3(
				floorf(vMinBound.x / m_vGridSize.x) * m_vGridSize.x,
				floorf(vMinBound.y / m_vGridSize.y) * m_vGridSize.y,
				floorf(vMinBound.z / m_vGridSize.z) * m_vGridSize.z
			);

			_float3 vMaxKey = _float3(
				floorf(vMaxBound.x / m_vGridSize.x) * m_vGridSize.x,
				floorf(vMaxBound.y / m_vGridSize.y) * m_vGridSize.y,
				floorf(vMaxBound.z / m_vGridSize.z) * m_vGridSize.z
			);

			for (_float x = vMinKey.x; x <= vMaxKey.x; x += m_vGridSize.x)
			{
				for (_float y = vMinKey.y; y <= vMaxKey.y; y += m_vGridSize.y)
				{
					for (_float z = vMinKey.z; z <= vMaxKey.z; z += m_vGridSize.z)
					{
						auto& GridLayer = m_arrGridLayers[IDX(_eScene)];
						if (GridLayer[_strGridLayerTag].end() == GridLayer[_strGridLayerTag].find(_float3(x, y, z)))
						{
							GridLayer[_strGridLayerTag][_float3(x, y, z)] = CGrid::Create(_float3(x, y, z), m_vGridSize);
						}

						GridLayer[_strGridLayerTag][_float3(x, y, z)]->Add_Polygon(pairVertices.first, _uint3(_i0, _i1, _i2));
					}
				}
			}

			return false;
		}
	);

	return S_OK;
}

HRESULT CGrid_Manager::Reset_Grids(const SCENE _eScene, const wstring& _strGridLayerTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CGrid_Manager::Reset_Grids", "Invalid Range: SCENE");
	}

	if (_strGridLayerTag.empty())
	{
		m_arrGridLayers[IDX(_eScene)].clear();
	}
	else
	{
		if (m_arrGridLayers[IDX(_eScene)].end() == m_arrGridLayers[IDX(_eScene)].find(_strGridLayerTag))
		{
			return S_FALSE;
		}

		m_arrGridLayers[IDX(_eScene)][_strGridLayerTag].clear();
	}

	return S_OK;
}

_float3 CGrid_Manager::Raycast(_vectorf _vRayOrigin, _vectorf _vRayDirection, _float _fRange)
{
	m_mmapRaycastGrid.clear();

	_vector vRayDirection = XMVector3Normalize(_vRayDirection);

	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		for (auto& layer : m_arrGridLayers[i])
		{
			for (auto& grid : layer.second)
			{
				DirectX::BoundingBox aabbGrid(grid.first + m_vGridSize * .5f, m_vGridSize * .5f);

				_float fDist = 0.f;
				if (aabbGrid.Intersects(_vRayOrigin, vRayDirection, fDist))
				{
					if (fDist < _fRange)
					{
						m_mmapRaycastGrid.emplace(fDist, grid.second);
					}
				}
			}
		}
	}

	return _vRayOrigin + vRayDirection * Raycast_Distance(_vRayOrigin, vRayDirection, _fRange);
}

_float3 CGrid_Manager::Raycast(const wstring& _strGridLayerTag, _vectorf _vRayOrigin, _vectorf _vRayDirection, _float _fRange)
{
	m_mmapRaycastGrid.clear();

	_vector vRayDirection = XMVector3Normalize(_vRayDirection);

	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		if (m_arrGridLayers[i].end() == m_arrGridLayers[i].find(_strGridLayerTag))
		{
			continue;
		}

		for (auto& pair : m_arrGridLayers[i][_strGridLayerTag])
		{
			DirectX::BoundingBox aabbGrid(pair.first + m_vGridSize * .5f, m_vGridSize * .5f);

			_float fDist = 0.f;
			if (aabbGrid.Intersects(_vRayOrigin, vRayDirection, fDist))
			{
				if (fDist < _fRange)
				{
					m_mmapRaycastGrid.emplace(fDist, pair.second);
				}
			}
		}
	}

	return _vRayOrigin + vRayDirection * Raycast_Distance(_vRayOrigin, vRayDirection, _fRange);
}

_float CGrid_Manager::Raycast_Distance(_vectorf _vRayOrigin, _vectorf _vRayDirection, _float _fRange)
{
	_float fFinalDist = FLT_MAX;

	for (auto& pair : m_mmapRaycastGrid)
	{
		pair.second->Iterate_Polygon(
			[&](_float3 _v0, _float3 _v1, _float3 _v2)->_bool
			{
				_float fDist = 0.f;
				if (TriangleTests::Intersects(_vRayOrigin, _vRayDirection, _v0, _v1, _v2, fDist))
				{
					if (fDist < _fRange && fDist < fFinalDist)
					{
						fFinalDist = fDist;
					};
				}

				return false;
			}
		);

		if (fFinalDist != FLT_MAX)
		{
			break;
		}
	}

	return fFinalDist == FLT_MAX ? 0.f : fFinalDist;
}
