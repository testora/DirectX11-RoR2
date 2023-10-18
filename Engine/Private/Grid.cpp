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
	m_vecNormal.emplace_back(Function::ComputeNormal(_polygon[0], _polygon[1], _polygon[2]));
}

void CGrid::Iterate_Polygon(function<_bool(POLYGON, _float3)> _fpCallback)
{
	for (size_t i = 0; i < m_vecPolygons.size(); ++i)
	{
		if (!_fpCallback(m_vecPolygons[i], m_vecNormal[i]))
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
