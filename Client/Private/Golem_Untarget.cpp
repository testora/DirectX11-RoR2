#include "ClientPCH.h"
#include "Golem_Untarget.h"
#include "Golem.h"

HRESULT CGolem_Untarget::Initialize(shared_ptr<CGolem> _pOwner)
{
	if (nullptr == _pOwner)
	{
		MSG_RETURN(E_FAIL, "CGolem_Untarget", "Nullptr Exception : _pOwner");
	}
	m_pGolem = _pOwner;

	return S_OK;
}

void CGolem_Untarget::Tick(_float _fTimeDelta)
{
}

void CGolem_Untarget::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CGolem_Untarget::Render()
{
	return S_OK;
}

shared_ptr<CGolem_Untarget> CGolem_Untarget::Create(shared_ptr<CGolem> _pOwner)
{
	shared_ptr<CGolem_Untarget> pInstance = make_private_shared(CGolem_Untarget);

	if (FAILED(pInstance->Initialize(_pOwner)))
	{
		MSG_RETURN(nullptr, "CGolem_Untarget::Create", "Failed To Initialize");
	}

	return pInstance;
}
