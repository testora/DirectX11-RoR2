#include "EnginePCH.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "Object_Manager.h"

CObjectPool::CObjectPool(const _uint _iPoolSize)
	: m_iPoolSize(_iPoolSize)
{
}

HRESULT CObjectPool::Initialize(const wstring& _strPrototype, std::any _arg)
{
	m_pPrototype = CObject_Manager::Get_Instance()->Clone_GameObject(_strPrototype);

	m_fnPush = [&]()-> shared_ptr<CGameObject> { return m_pPrototype->Clone(_arg); };

	for (size_t i = 0; i < m_iPoolSize; ++i)
	{
		Push();
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
}

shared_ptr<CGameObject> CObjectPool::Pop()
{
	if (m_deqPool.empty())
	{
		Push();
	}

	shared_ptr<CGameObject> pObject = m_deqPool.front();
	pObject->Fetch();
	m_deqPool.pop_front();

	return pObject;
}

void CObjectPool::Push(shared_ptr<CGameObject> _pObject)
{
	if (nullptr == _pObject)
	{
		shared_ptr<CGameObject> pObject = m_fnPush();
		if (nullptr != pObject)
		{
			m_deqPool.emplace_back(pObject);
		}
	}
	else
	{
		m_usetPop.erase(_pObject);
		m_deqPool.emplace_back(_pObject);
	}
}

shared_ptr<CObjectPool> CObjectPool::Create(const _uint _iPoolSize, const wstring& _strPrototypeTag, std::any _arg)
{
	shared_ptr<CObjectPool> pInstance = make_private_shared(CObjectPool, _iPoolSize);

	if (FAILED(pInstance->Initialize(_strPrototypeTag, _arg)))
	{
		MSG_RETURN(nullptr, "CObject::Create", "Failed to Initialize");
	}

	return pInstance;
}
