#include "ClientPCH.h"
#include "Golem_Hit.h"
#include "Golem.h"

HRESULT CGolem_Hit::Initialize(shared_ptr<CGolem> _pOwner)
{
	if (nullptr == _pOwner)
	{
		MSG_RETURN(E_FAIL, "CGolem_Hit", "Nullptr Exception : _pOwner");
	}
	m_pGolem = _pOwner;

	return S_OK;
}

void CGolem_Hit::Tick(_float _fTimeDelta)
{
}

void CGolem_Hit::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CGolem_Hit::Render()
{
    return S_OK;
}

shared_ptr<CGolem_Hit> CGolem_Hit::Create(shared_ptr<CGolem> _pOwner)
{
	shared_ptr<CGolem_Hit> pInstance = make_private_shared(CGolem_Hit);

	if (FAILED(pInstance->Initialize(_pOwner)))
	{
		MSG_RETURN(nullptr, "CGolem_Hit::Create", "Failed To Initialize");
	}

	return pInstance;
}
