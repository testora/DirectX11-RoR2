#include "EnginePCH.h"
#include "ObjectPool.h"

CObjectPool::CObjectPool(const _uint _iPoolSize)
	: m_iPoolSize(_iPoolSize)
{
}

HRESULT CObjectPool::Initialize(shared_ptr<class CGameObject> _pObject)
{
    return S_OK;
}

void CObjectPool::Tick(_float _fTimeDelta)
{
}

void CObjectPool::Late_Tick(_float _fTimeDelta)
{
}

shared_ptr<class CGameObject> CObjectPool::Pop()
{
    return shared_ptr<class CGameObject>();
}

void CObjectPool::Push(shared_ptr<class CGameObject>)
{
}
