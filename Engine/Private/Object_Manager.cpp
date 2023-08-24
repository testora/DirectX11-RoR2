#include "EnginePCH.h"
#include "Object_Manager.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "Scene_Manager.h"

HRESULT CObject_Manager::Reserve_Manager(const SCENE _eSceneMax)
{
	m_arrPrototypes	= Function::CreateDynamicArray<Prototypes>(IDX(_eSceneMax), false);
	m_arrLayers		= Function::CreateDynamicArray<Layers>(IDX(_eSceneMax), false);
	m_arrPools		= Function::CreateDynamicArray<Pools>(IDX(_eSceneMax), false);

	m_eSceneMax		= _eSceneMax;

	return S_OK;
}

void CObject_Manager::Tick(_float _fTimeDelta)
{
	for (auto& iter : m_arrLayers[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Tick(_fTimeDelta);
	}
	for (auto& iter : m_arrPools[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Tick(_fTimeDelta);
	}
}

void CObject_Manager::Late_Tick(_float _fTimeDelta)
{
	for (auto& iter : m_arrLayers[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Late_Tick(_fTimeDelta);
	}
	for (auto& iter : m_arrPools[IDX(CScene_Manager::Get_Instance()->Current_Scene())])
	{
		iter.second->Late_Tick(_fTimeDelta);
	}
}

shared_ptr<CObjectLayer> CObject_Manager::Find_Layer(const SCENE _eScene, const wstring& _strLayerTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Find_Layer", "Invalid Range: SCENE");
	}

	auto& Prototype = m_arrLayers[IDX(_eScene)];
	auto iter = Prototype.find(_strLayerTag);
	if (iter == Prototype.end())
	{
		return nullptr;
	}

	return iter->second;
}

shared_ptr<CObjectPool> CObject_Manager::Find_Pool(const SCENE _eScene, const wstring& _strPoolTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Find_Pool", "Invalid Range: SCENE");
	}

	auto& Prototype = m_arrPools[IDX(_eScene)];
	auto iter = Prototype.find(_strPoolTag);
	if (iter == Prototype.end())
	{
		return nullptr;
	}

	return iter->second;
}

HRESULT CObject_Manager::Add_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag, shared_ptr<CGameObject> _pPrototype)
{
	if (nullptr != Find_Prototype(_eScene, _strPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Add_Prototype", "Already Exists: CGameObject");
	}

	m_arrPrototypes[IDX(_eScene)].emplace(_strPrototypeTag, _pPrototype);

	return S_OK;
}

shared_ptr<CObjectLayer> CObject_Manager::Add_Layer(const SCENE _eScene, const wstring& _strLayerTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Invalid Range: SCENE");
	}

	if (nullptr != Find_Layer(_eScene, _strLayerTag))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Already Exists: CObjectLayer");
	}

	shared_ptr<CObjectLayer> pLayer = CObjectLayer::Create(_eScene);
	if (nullptr == pLayer)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Failed to Create");
	}

	m_arrLayers[IDX(_eScene)].emplace(_strLayerTag, pLayer);

	return pLayer;
}

shared_ptr<CObjectPool> CObject_Manager::Add_Pool(const SCENE _eScene, const wstring& _strPoolTag, const wstring& _strPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Invalid Range: SCENE");
	}

	if (nullptr != Find_Pool(_eScene, _strPoolTag))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Already Exists: CObjectPool");
	}

	shared_ptr<CObjectPool> pPool = CObjectPool::Create(_eScene, _iPoolSize, _strPrototypeTag, _arg);
	if (nullptr == pPool)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Failed to Create");
	}

	m_arrPools[IDX(_eScene)].emplace(_strPoolTag, pPool);

	return pPool;
}

shared_ptr<CGameObject> CObject_Manager::Clone_GameObject(const SCENE _eScene, const wstring& _strPrototypeTag, any _arg)
{
	shared_ptr<CGameObject> pGameObject = Find_Prototype(_eScene,_strPrototypeTag);
	if (nullptr == pGameObject)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Clone_GameObject", "Failed to Find_Prototype");
	}

	return pGameObject->Clone(_arg);
}

HRESULT CObject_Manager::Clear_Scene_Object(const SCENE _eScene)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Clear_Scene_Object", "Invalid Range: SCENE");
	}

	m_arrPrototypes[IDX(_eScene)].clear();
	m_arrLayers[IDX(_eScene)].clear();
	m_arrPools[IDX(_eScene)].clear();

	return S_OK;
}

shared_ptr<CGameObject> CObject_Manager::Find_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Find_Prototype", "Invalid Range: SCENE");
	}

	auto& Prototypes = m_arrPrototypes[IDX(_eScene)];
	auto iter = Prototypes.find(_strPrototypeTag);
	if (iter == Prototypes.end())
	{
		return nullptr;
	}

	return iter->second;
}
