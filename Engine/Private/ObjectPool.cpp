#include "EnginePCH.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "Object_Manager.h"

CObjectPool::CObjectPool(const SCENE _eScene, const _uint _iPoolSize)
	: m_iPoolSize(_iPoolSize)
	, m_eScene(_eScene)
{
}

HRESULT CObjectPool::Initialize(const wstring& _wstrPrototype, any _arg)
{
	m_pPrototype = CObject_Manager::Get_Instance()->Clone_GameObject(m_eScene, _wstrPrototype);

	m_funcPush = [&]()->shared_ptr<CGameObject> { return m_pPrototype->Clone(_arg); };

	for (size_t i = 0; i < m_iPoolSize; ++i)
	{
		Add();
	}

	return S_OK;
}

void CObjectPool::Tick(_float _fTimeDelta)
{
	for (auto& iter : m_usetPop)
	{
		iter->Tick(_fTimeDelta);
	}
}

void CObjectPool::Late_Tick(_float _fTimeDelta)
{
	for (auto& iter : m_usetPop)
	{
		iter->Late_Tick(_fTimeDelta);
	}
	for (auto& iter : m_usetPop)
	{
		if (iter->Return())
		{
			if (FAILED(Push(iter)))
			{
				MSG_RETURN(, "CObjectPool::Late_Tick", "Failed to Push");
			}
		}
	}

	for (auto& iter : m_lstPush)
	{
		m_usetPop.erase(iter);
		m_deqPool.emplace_back(iter);
	}
	for (auto& iter : m_lstPop)
	{
		m_usetPop.emplace(iter);
	}
	m_lstPush.clear();
	m_lstPop.clear();
}

shared_ptr<CGameObject> CObjectPool::Pop(any _aFetchArg)
{
	if (m_deqPool.empty())
	{
		Add();
	}

	shared_ptr<CGameObject> pObject = m_deqPool.front();
	pObject->Fetch(_aFetchArg);

	m_deqPool.pop_front();
	m_lstPop.emplace_back(pObject);

	return pObject;
}

HRESULT CObjectPool::Push(shared_ptr<CGameObject> _pObject)
{
	if (nullptr == _pObject)
	{
		MSG_RETURN(E_FAIL, "CObjectPool::Push", "Null Exception");
	}

	m_lstPush.emplace_back(_pObject);

	return S_OK;
}

void CObjectPool::Iterate_Objects(function<_bool(shared_ptr<CGameObject>)> _funcCallback)
{
	for (auto iter : m_usetPop)
	{
		if (!_funcCallback(iter))
		{
			return;
		}
	}
}

void CObjectPool::Add()
{
	shared_ptr<CGameObject> pObject = m_funcPush();
	if (nullptr != pObject)
	{
		m_deqPool.emplace_back(pObject);
	}
}

shared_ptr<CObjectPool> CObjectPool::Create(const SCENE _eScene, const _uint _iPoolSize, const wstring& _wstrPrototypeTag, any _arg)
{
	shared_ptr<CObjectPool> pInstance = make_private_shared(CObjectPool, _eScene, _iPoolSize);

	if (FAILED(pInstance->Initialize(_wstrPrototypeTag, _arg)))
	{
		MSG_RETURN(nullptr, "CObject::Create", "Failed to Initialize");
	}

	return pInstance;
}
