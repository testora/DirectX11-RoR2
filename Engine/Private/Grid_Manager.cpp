#include "EnginePCH.h"
#include "Grid_Manager.h"
#include "Grid.h"
#include "GameObject.h"
#include "VIBuffer.h"
#include "Transform.h"

HRESULT CGrid_Manager::Initialize(_float3 _vGridSize)
{
	m_vGridSize = _vGridSize;

	return S_OK;
}

void CGrid_Manager::Register_VIBuffer(shared_ptr<CGameObject> _pGameObject)
{
	if (nullptr == _pGameObject)
	{
		MSG_RETURN(, "CGrid_Manager::Register_VIBuffer", "Nullptr Exception");
	}

	shared_ptr<CTransform>	pTransform	= _pGameObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	shared_ptr<CVIBuffer>	pVIBuffer	= _pGameObject->Get_Component<CVIBuffer>(COMPONENT::VIBUFFER);

	pVIBuffer->Iterate_Indices(
		[&](_uint _i0, _uint _i1, _uint _i2)->_bool
		{
			pair<_float3*, _uint> pairVertices = pVIBuffer->Get_Vertices();

			_float3 arrVertex[3] =
			{
				 XMVector3TransformCoord(pairVertices.first[_i0], pTransform->Get_Matrix()),
				 XMVector3TransformCoord(pairVertices.first[_i1], pTransform->Get_Matrix()),
				 XMVector3TransformCoord(pairVertices.first[_i2], pTransform->Get_Matrix())
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
						if (nullptr == m_umapGrid[_float3(x, y, z)])
						{
							m_umapGrid[_float3(x, y, z)] = CGrid::Create(_float3(x, y, z), m_vGridSize);
						}
					
						m_umapGrid[_float3(x, y, z)]->Add_Polygon(pairVertices.first, _uint3(_i0, _i1, _i2));
					}
				}
			}

			return false;
		}
	);
}

void CGrid_Manager::Reset_Grids()
{
	m_umapGrid.clear();
}

_float3 CGrid_Manager::Raycast(_vectorf _vRayOrigin, _vectorf _vRayDirection)
{
	_vector vRayDirection	= XMVector3Normalize(_vRayDirection);

	for (auto& pair : m_umapGrid)
	{
		DirectX::BoundingBox aabbGrid(pair.first + m_vGridSize * .5f, m_vGridSize * .5f);

		_float fDist = 0.f;
		if (aabbGrid.Intersects(_vRayOrigin, vRayDirection, fDist))
		{
			m_mapRaycastGrid.emplace(fDist, pair.second);
		}
	}

	_float fFinalDist = FLT_MAX;

	for (auto& pair : m_mapRaycastGrid)
	{
		pair.second->Iterate_Polygon(
			[&](_float3 _v0, _float3 _v1, _float3 _v2)->_bool
			{
				_float fDist = 0.f;
				if (TriangleTests::Intersects(_vRayOrigin, vRayDirection, _v0, _v1, _v2, fDist))
				{
					if (fDist < fFinalDist)
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

	m_mapRaycastGrid.clear();

	return _vRayOrigin + vRayDirection * (fFinalDist == FLT_MAX ? 0.f : fFinalDist);
}
