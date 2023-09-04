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

	void															Tick(_float fTimeDelta);
	void															Late_Tick(_float fTimeDelta);

public:
	shared_ptr<CObjectLayer>										Find_Layer(const SCENE, const wstring& wstrLayerTag);
	shared_ptr<CObjectPool>											Find_Pool(const SCENE, const wstring& wstrPoolTag);

public:
	HRESULT															Add_Prototype(const SCENE, const wstring& wstrPrototypeTag, shared_ptr<class CGameObject> pPrototype);

	shared_ptr<CObjectLayer>										Add_Layer(const SCENE, const wstring& wstrLayerTag);
	shared_ptr<CObjectPool>											Add_Pool(const SCENE, const wstring& wstrPoolTag, const wstring& wstrPrototypeTag, _uint iPoolSize, any = g_aNull);

	shared_ptr<CGameObject>											Clone_GameObject(const SCENE, const wstring& wstrPrototypeTag, any = g_aNull);

	HRESULT															Clear_Scene_Object(const SCENE);

	void															Iterate_Layers(const SCENE, function<_bool(pair<wstring, shared_ptr<CObjectLayer>>)>);
	void															Iterate_Pools(const SCENE, function<_bool(pair<wstring, shared_ptr<CObjectPool>>)>);

private:
	shared_ptr<CGameObject>											Find_Prototype(const SCENE, const wstring& wstrPrototypeTag);

private:
	typedef unordered_map<wstring, shared_ptr<CGameObject>>			Prototypes;
	typedef unordered_map<wstring, shared_ptr<CObjectLayer>>		Layers;
	typedef unordered_map<wstring, shared_ptr<CObjectPool>>			Pools;

	unique_ptr<Prototypes[]>										m_arrPrototypes;
	unique_ptr<Layers[]>											m_arrLayers;
	unique_ptr<Pools[]>												m_arrPools;

	SCENE															m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CObject_Manager>;
};

END
