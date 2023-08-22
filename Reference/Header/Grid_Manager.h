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
	HRESULT																Register_VIBuffer(const SCENE, const wstring& strGridLayerTag, shared_ptr<class CGameObject>);
	HRESULT																Register_VIBuffer(const SCENE, const wstring& strGridLayerTag, shared_ptr<class CVIBuffer>, shared_ptr<class CTransform>);
	HRESULT																Reset_Grids(const SCENE, const wstring& strGridLayerTag = wstring());

	_float3																Raycast(_vectorf vRayOrigin, _vectorf vRayDirection, _float fRange = FLT_MAX);
	_float3																Raycast(const wstring& strGridLayerTag, _vectorf vRayOrigin, _vectorf vRayDirection, _float fRange = FLT_MAX);

private:
	_float																Raycast_Distance(_vectorf vRayOrigin, _vectorf vRayDirection, _float fRange = FLT_MAX);

private:
	typedef unordered_map<_float3, shared_ptr<class CGrid>, FLOAT3HASH>	Grids;
	typedef unordered_map<wstring, Grids>								GridLayers;

	unique_ptr<GridLayers[]>											m_arrGridLayers;

	multimap<_float, shared_ptr<class CGrid>>							m_mmapRaycastGrid;

	_float3																m_vGridSize;

	SCENE																m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CGrid_Manager>;
};

END
