#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CGrid_Manager final : public CSingleton<CGrid_Manager>
{
private:
	explicit CGrid_Manager() DEFAULT;
	virtual ~CGrid_Manager() DEFAULT;

public:
	HRESULT														Initialize(_float3 vGridSize);

public:
	_float3														Get_GridSize() const	{ return m_vGridSize; }

public:
	void														Register_VIBuffer(shared_ptr<class CGameObject>);
	void														Reset_Grids();

	_float3														Raycast(_vectorf vRayOrigin, _vectorf vRayDirection);

private:
	unordered_map<_float3, shared_ptr<class CGrid>, FLOAT3HASH>	m_umapGrid;

	map<_float, shared_ptr<class CGrid>>						m_mapRaycastGrid;

	_float3														m_vGridSize;

	friend CSingleton<CGrid_Manager>;
};

END
