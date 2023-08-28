#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CGrid final
{
private:
	explicit CGrid() DEFAULT;
	virtual ~CGrid() DEFAULT;

public:
	HRESULT									Initialize(_float3 vMinBound, _float3 vGridSize);

public:
	void									Add_Polygon(const _float3* pVertices, _uint3 vIndices);

	void									Iterate_Polygon(function<_bool(_float3 v0, _float3 v1, _float3 v2)>);

private:
	typedef vector<_uint3>					POLYGONS;
	unordered_map<const _float3*, POLYGONS>	m_umapPolygons;

	_float3									m_vMinBound;
	_float3									m_vGridSize;

public:
	static shared_ptr<CGrid>				Create(_float3 vMinBound, _float3 vGridSize);
};

END
