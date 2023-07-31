#include "EnginePCH.h"
#include "ObjectLayer.h"

CObjectLayer::CObjectLayer()
{
}

void CObjectLayer::Tick(_float fTimeDelta)
{
}

void CObjectLayer::Late_Tick(_float fTimeDelta)
{
}

HRESULT CObjectLayer::Add(shared_ptr<class CGameObject>)
{
	return S_OK;
}

HRESULT CObjectLayer::Delete()
{
	return S_OK;
}
