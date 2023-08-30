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
	void									Add_Polygon(POLYGON);

	void									Iterate_Polygon(function<_bool(POLYGON)>);

private:
	vector<POLYGON>							m_vecPolygons;

	_float3									m_vMinBound;
	_float3									m_vGridSize;

public:
	static shared_ptr<CGrid>				Create(_float3 vMinBound, _float3 vGridSize);
};

END
