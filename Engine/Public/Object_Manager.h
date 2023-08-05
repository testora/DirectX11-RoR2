#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CObject_Manager final : public CSingleton<CObject_Manager>
{
private:
	explicit CObject_Manager() DEFAULT;
	virtual ~CObject_Manager() DEFAULT;

public:
	HRESULT															Reserve_Manager(const SCENE);

public:
	void															Tick(_float fTimeDelta);
	void															Late_Tick(_float fTimeDelta);

public:
	shared_ptr<class CObjectLayer>									Find_Layer(const SCENE, const wstring& strLayerTag);
	shared_ptr<class CObjectPool>									Find_Pool(const SCENE, const wstring& strPoolTag);

public:
	HRESULT															Add_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<class CGameObject> pPrototype);

	shared_ptr<class CObjectLayer>									Add_Layer(const SCENE, const wstring& strLayerTag);
	shared_ptr<class CObjectPool>									Add_Pool(const SCENE, const wstring& strPoolTag, const wstring& strPrototypeTag, _uint iPoolSize, any = any());

	shared_ptr<class CGameObject>									Clone_GameObject(const SCENE, const wstring& strPrototypeTag, any = any());

	HRESULT															Clear_Scene_Object(const SCENE);

private:
	shared_ptr<class CGameObject>									Find_Prototype(const SCENE, const wstring& strPrototypeTag);

private:
	typedef unordered_map<wstring, shared_ptr<class CGameObject>>	Prototype;
	typedef unordered_map<wstring, shared_ptr<class CObjectLayer>>	Layer;
	typedef unordered_map<wstring, shared_ptr<class CObjectPool>>	Pool;

	unique_ptr<Prototype[]>											m_arrPrototypes;
	unique_ptr<Layer[]>												m_arrLayers;
	unique_ptr<Pool[]>												m_arrPools;

	friend CSingleton<CObject_Manager>;
};

END
