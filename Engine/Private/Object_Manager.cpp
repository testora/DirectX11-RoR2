#include "EnginePCH.h"
#include "Object_Manager.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "Scene_Manager.h"

HRESULT CObject_Manager::Reserve_Manager(const SCENE _eScene)
{
	m_arrPrototypes	= Function::MakeUniqueDynamicArray<Prototype>(IDX(_eScene), false);
	m_arrLayers		= Function::MakeUniqueDynamicArray<Layer>(IDX(_eScene), false);
	m_arrPools		= Function::MakeUniqueDynamicArray<Pool>(IDX(_eScene), false);

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

shared_ptr<class CObjectLayer> CObject_Manager::Find_Layer(const SCENE _eScene, const wstring& _strLayerTag)
{
	auto& Prototype = m_arrLayers[IDX(_eScene)];
	auto iter = Prototype.find(_strLayerTag);
	if (iter == Prototype.end())
	{
		return nullptr;
	}

	return iter->second;
}

shared_ptr<class CObjectPool> CObject_Manager::Find_Pool(const SCENE _eScene, const wstring& _strPoolTag)
{
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

shared_ptr<class CObjectLayer> CObject_Manager::Add_Layer(const SCENE _eScene, const wstring& _strLayerTag)
{
	if (nullptr != Find_Layer(_eScene, _strLayerTag))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Already Exists: CObjectLayer");
	}

	shared_ptr<class CObjectLayer> pLayer = CObjectLayer::Create(_eScene);
	if (nullptr == pLayer)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Layer", "Failed to Create");
	}

	m_arrLayers[IDX(_eScene)].emplace(_strLayerTag, pLayer);

	return pLayer;
}

shared_ptr<class CObjectPool> CObject_Manager::Add_Pool(const SCENE _eScene, const wstring& _strPoolTag, const wstring& _strPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (nullptr != Find_Pool(_eScene, _strPoolTag))
	{
		MSG_RETURN(nullptr, "CObject_Manager::Add_Pool", "Already Exists: CObjectPool");
	}

	shared_ptr<class CObjectPool> pPool = CObjectPool::Create(_eScene, _iPoolSize, _strPrototypeTag, _arg);
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
	m_arrPrototypes[IDX(_eScene)].clear();
	m_arrLayers[IDX(_eScene)].clear();
	m_arrPools[IDX(_eScene)].clear();

	return S_OK;
}

shared_ptr<CGameObject> CObject_Manager::Find_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag)
{
	auto& Prototype = m_arrPrototypes[IDX(_eScene)];
	auto iter = Prototype.find(_strPrototypeTag);
	if (iter == Prototype.end())
	{
		return nullptr;
	}

	return iter->second;
}
