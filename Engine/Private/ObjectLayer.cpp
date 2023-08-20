#include "EnginePCH.h"
#include "ObjectLayer.h"
#include "GameObject.h"

CObjectLayer::CObjectLayer(const SCENE _eScene)
	: m_eScene(_eScene)
{
}

void CObjectLayer::Tick(_float _fTimeDelta)
{
	for (auto& pObject : m_usetObject)
	{
		pObject->Tick(_fTimeDelta);
	}
}

void CObjectLayer::Late_Tick(_float _fTimeDelta)
{
	for (auto& pObject : m_usetObject)
	{
		pObject->Late_Tick(_fTimeDelta);
	}
}

HRESULT CObjectLayer::Add(shared_ptr<class CGameObject> _pObject)
{
	if (nullptr == _pObject)
	{
		MSG_RETURN(E_FAIL, "CObjectLayer::Add", "Null Exception");
	}

	m_usetObject.emplace(_pObject);

	return S_OK;
}

HRESULT CObjectLayer::Delete(shared_ptr<class CGameObject> _pObject)
{
	auto iter = m_usetObject.find(_pObject);
	if (iter == m_usetObject.end())
	{
		MSG_RETURN(E_FAIL, "CObjectLayer::Delete", "Failed to find");
	}

	m_usetObject.erase(iter);

	return S_OK;
}

void CObjectLayer::Iterate_Objects(function<_bool(shared_ptr<class CGameObject>)> _fn)
{
	for (auto iter : m_usetObject)
	{
		if (_fn(iter))
		{
			return;
		};
	}
}

shared_ptr<CObjectLayer> CObjectLayer::Create(const SCENE _eScene)
{
	return make_private_shared(CObjectLayer, _eScene);
}
