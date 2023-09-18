#include "ClientPCH.h"
#include "Golem_Target.h"
#include "Golem.h"

HRESULT CGolem_Target::Initialize(shared_ptr<CGolem> _pOwner)
{
	if (nullptr == _pOwner)
	{
		MSG_RETURN(E_FAIL, "CGolem_Target", "Nullptr Exception : _pOwner");
	}
	m_pGolem = _pOwner;

	return S_OK;
}

void CGolem_Target::Tick(_float _fTimeDelta)
{
}

void CGolem_Target::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CGolem_Target::Render()
{
	return S_OK;
}

shared_ptr<CGolem_Target> CGolem_Target::Create(shared_ptr<CGolem>_pOwner)
{
	shared_ptr<CGolem_Target> pInstance = make_private_shared(CGolem_Target);

	if (FAILED(pInstance->Initialize(_pOwner)))
	{
		MSG_RETURN(nullptr, "CGolem_Target::Create", "Failed To Initialize");
	}

	return pInstance;
}
