#include "EnginePCH.h"
#include "Object_Manager.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#include "GameObject.h"

void CObject_Manager::Tick(_float _fTimeDelta)
{
	for (auto& iter : m_umapLayer)
	{
		iter.second->Tick(_fTimeDelta);
	}

	for (auto& iter : m_umapPool)
	{
		iter.second->Tick(_fTimeDelta);
	}
}

void CObject_Manager::Late_Tick(_float _fTimeDelta)
{
	for (auto& iter : m_umapLayer)
	{
		iter.second->Late_Tick(_fTimeDelta);
	}

	for (auto& iter : m_umapPool)
	{
		iter.second->Late_Tick(_fTimeDelta);
	}
}

shared_ptr<class CObjectLayer> CObject_Manager::Find_Layer(const wstring& _strLayerTag)
{
	auto iter = m_umapLayer.find(_strLayerTag);
	if (iter == m_umapLayer.end())
	{
		return nullptr;
	}

	return m_umapLayer[_strLayerTag];
}

shared_ptr<class CObjectPool> CObject_Manager::Find_Pool(const wstring& _strPoolTag)
{
	auto iter = m_umapPool.find(_strPoolTag);
	if (iter == m_umapPool.end())
	{
		return nullptr;
	}

	return m_umapPool[_strPoolTag];
}

HRESULT CObject_Manager::Add_Prototype(const wstring& _strPrototypeTag, shared_ptr<CGameObject> _pPrototype)
{
	if (nullptr != Find_Prototype(_strPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Add_Prototype", "Already Exists: CGameObject");
	}

	m_umapPrototype.emplace(_strPrototypeTag, _pPrototype);

	return S_OK;
}

HRESULT CObject_Manager::Add_Layer(const wstring& _strLayerTag)
{
	if (nullptr != Find_Layer(_strLayerTag))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Add_Layer", "Already Exists: CObjectLayer");
	}

	m_umapLayer.emplace(_strLayerTag, CObjectLayer::Create());

	return S_OK;
}

HRESULT CObject_Manager::Add_Pool(const wstring& _strPoolTag, const wstring& _strPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (nullptr != Find_Pool(_strPoolTag))
	{
		MSG_RETURN(E_FAIL, "CObject_Manager::Add_Pool", "Already Exists: CObjectPool");
	}

	m_umapPool.emplace(_strPoolTag, CObjectPool::Create(_iPoolSize, _strPrototypeTag, _arg));

	return S_OK;
}

shared_ptr<CGameObject> CObject_Manager::Clone_GameObject(const wstring& _strPrototypeTag, any _arg)
{
	shared_ptr<CGameObject> pGameObject = Find_Prototype(_strPrototypeTag);
	if (nullptr == pGameObject)
	{
		MSG_RETURN(nullptr, "CObject_Manager::Clone_GameObject", "Failed to Find_Prototype");
	}

	return pGameObject->Clone(_arg);
}

shared_ptr<CGameObject> CObject_Manager::Find_Prototype(const wstring& _strPrototypeTag)
{
	auto iter = m_umapPrototype.find(_strPrototypeTag);
	if (iter == m_umapPrototype.end())
	{
		return nullptr;
	}

	return m_umapPrototype[_strPrototypeTag];
}
