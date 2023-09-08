#include "ClientPCH.h"
#include "Control_RailGunner.h"
#include "RailGunner_Crosshair.h"
#include "RailGunner.h"
#include "GameInstance.h"

HRESULT CControl_RailGunner::Initialize(shared_ptr<class CGameObject> _pOwner, shared_ptr<CRailGunner_Crosshair> _pCrosshair, const CHARACTERDESC* _pCharacterDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pCharacterDesc)))
	{
		MSG_RETURN(E_FAIL, "CControl_RailGunner::Initialize", "Failed to __super::Initialize");
	}

	m_pRailGunner	= dynamic_pointer_cast<CRailGunner>(_pOwner);
	if (nullptr == m_pRailGunner)
	{
		MSG_RETURN(E_FAIL, "CControl_RailGunner::Initialize", "Nullptr Exception: m_pRailGunner");
	}

	m_pCrosshair	= _pCrosshair;
	if (nullptr == m_pCrosshair)
	{
		MSG_RETURN(E_FAIL, "CControl_RailGunner::Initialize", "Nullptr Exception: m_pCrosshair");
	}

	return S_OK;
}

void CControl_RailGunner::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CControl_RailGunner::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

void CControl_RailGunner::Handle_MouseInput(_float _fTimeDelta)
{
	
}

void CControl_RailGunner::Handle_KeyInput(_float _fTimeDelta)
{

}

shared_ptr<CControl_RailGunner> CControl_RailGunner::Create(shared_ptr<class CGameObject> _pOwner, shared_ptr<CRailGunner_Crosshair> _pCrosshair, const CHARACTERDESC* _pCharacterDesc)
{
	shared_ptr<CControl_RailGunner> pInstance = make_private_shared(CControl_RailGunner);

	if (FAILED(pInstance->Initialize(_pOwner, _pCrosshair, _pCharacterDesc)))
	{
		MSG_RETURN(nullptr, "CControl_RailGunner::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CControl_RailGunner::Clone(any)
{
	return make_private_shared_copy(CControl_RailGunner, *this);
}
