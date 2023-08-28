#include "EnginePCH.h"
#include "Grid.h"

HRESULT CGrid::Initialize(_float3 _vMinBound, _float3 _vGridSize)
{
	m_vMinBound	= _vMinBound;
	m_vGridSize	= _vGridSize;

	return S_OK;
}

void CGrid::Add_Polygon(const _float3* _pVertices, _uint3 _vIndices)
{
	m_umapPolygons[_pVertices].emplace_back(_vIndices);
}

void CGrid::Iterate_Polygon(function<_bool(_float3, _float3, _float3)> _funcCallback)
{
	for (auto& pair : m_umapPolygons)
	{
		for (auto& vIndices : pair.second)
		{
			if (!_funcCallback(pair.first[vIndices.x], pair.first[vIndices.y], pair.first[vIndices.z]))
			{
				return;
			}
		}
	}
}

shared_ptr<CGrid> CGrid::Create(_float3 _vMinBound, _float3 _vGridSize)
{
	shared_ptr<CGrid> pInstance = make_private_shared(CGrid);

	if (FAILED(pInstance->Initialize(_vMinBound, _vGridSize)))
	{
		MSG_RETURN(nullptr, "CGrid::Create", "Failed to Initialize");
	}

	return pInstance;
}
