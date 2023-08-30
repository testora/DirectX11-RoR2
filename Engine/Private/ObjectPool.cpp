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
}

shared_ptr<CGameObject> CObjectPool::Pop()
{
	if (m_deqPool.empty())
	{
		Add();
	}

	shared_ptr<CGameObject> pObject = m_deqPool.front();
	pObject->Fetch(m_fetchArg);
	m_deqPool.pop_front();

	return pObject;
}

HRESULT CObjectPool::Push(shared_ptr<CGameObject> _pObject)
{
	if (nullptr == _pObject)
	{
		MSG_RETURN(E_FAIL, "CObjectPool::Push", "Null Exception");
	}

	m_usetPop.erase(_pObject);
	m_deqPool.emplace_back(_pObject);

	return S_OK;
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
