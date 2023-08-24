#pragma once
#include "Engine_Define.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#include "GameObject.h"

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
	typedef unordered_map<wstring, shared_ptr<class CGameObject>>	Prototypes;
	typedef unordered_map<wstring, shared_ptr<class CObjectLayer>>	Layers;
	typedef unordered_map<wstring, shared_ptr<class CObjectPool>>	Pools;

	unique_ptr<Prototypes[]>										m_arrPrototypes;
	unique_ptr<Layers[]>											m_arrLayers;
	unique_ptr<Pools[]>												m_arrPools;

	SCENE															m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CObject_Manager>;
};

END
