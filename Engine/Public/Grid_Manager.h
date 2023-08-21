#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CGrid_Manager final : public CSingleton<CGrid_Manager>
{
private:
	explicit CGrid_Manager() DEFAULT;
	virtual ~CGrid_Manager() DEFAULT;

public:
	HRESULT																Reserve_Manager(const SCENE, _float3 vGridSize);

public:
	_float3																Get_GridSize() const	{ return m_vGridSize; }

public:
	void																Register_VIBuffer(const SCENE, shared_ptr<class CGameObject>);
	void																Reset_Grids(const SCENE);

	_float3																Raycast(_vectorf vRayOrigin, _vectorf vRayDirection);

private:
	typedef unordered_map<_float3, shared_ptr<class CGrid>, FLOAT3HASH>	Grids;

	unique_ptr<Grids[]>													m_arrGrids;

	multimap<_float, shared_ptr<class CGrid>>							m_mmapRaycastGrid;

	_float3																m_vGridSize;

	SCENE																m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CGrid_Manager>;
};

END
