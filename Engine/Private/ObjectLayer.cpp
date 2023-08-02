#include "EnginePCH.h"
#include "ObjectLayer.h"
#include "GameObject.h"


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

shared_ptr<CObjectLayer> CObjectLayer::Create()
{
	return make_private_shared(CObjectLayer);
}
