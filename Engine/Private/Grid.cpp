#include "EnginePCH.h"
#include "Grid.h"

HRESULT CGrid::Initialize(_float3 _vMinBound, _float3 _vGridSize)
{
	m_vMinBound	= _vMinBound;
	m_vGridSize	= _vGridSize;
	
	return S_OK;
}

void CGrid::Add_Polygon(POLYGON _polygon)
{
	m_vecPolygons.emplace_back(_polygon);
}

void CGrid::Iterate_Polygon(function<_bool(POLYGON)> _funcCallback)
{
	for (auto& polygon : m_vecPolygons)
	{
		if (!_funcCallback(polygon))
		{
			return;
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
