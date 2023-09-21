#include "ClientPCH.h"
#include "Brother_Behavior.h"
#include "GameInstance.h"
#include "Brother.h"
#include "Brother_Phase_0.h"
#include "Brother_Phase_1.h"

HRESULT CBrother_Behavior::Initialize(shared_ptr<CGameObject> _pOwner, shared_ptr<CGameObject> _pTarget, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pTarget, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CBrother_Behavior::Initialize", "Failed to __super::Initialize");
	}

	m_ePhase	= BROTHER_PHASE::PHASE0;
	m_pPhase	= CBrother_Phase_0::Create(_pOwner, static_pointer_cast<CBrother_Behavior>(shared_from_behavior()), _pEntityDesc);

	return S_OK;
}

void CBrother_Behavior::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_pPhase->Tick(_fTimeDelta);
	m_pPhase->Late_Tick(_fTimeDelta);
}

void CBrother_Behavior::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

_bool CBrother_Behavior::Is_Target_InRange(_float _fRange) const
{
	return __super::Is_Target_InRange(_fRange);
}

void CBrother_Behavior::Change_Phase(BROTHER_PHASE _ePhase)
{
	switch (_ePhase)
	{
	case BROTHER_PHASE::PHASE1:
	{
		m_pPhase = CBrother_Phase_1::Create(m_pPhase);
	}
	break;
	case BROTHER_PHASE::PHASE2:
	{
	}
	break;
	case BROTHER_PHASE::PHASE3:
	{
	}
	break;
	case BROTHER_PHASE::PHASE4:
	{
	}
	break;
	}
}

shared_ptr<CBrother_Behavior> CBrother_Behavior::Create(shared_ptr<CGameObject> _pOwner, shared_ptr<CGameObject> _pTarget, const ENTITYDESC* _pEntityDesc)
{
	shared_ptr<CBrother_Behavior> pInstance = make_private_shared(CBrother_Behavior);

	if (FAILED(pInstance->Initialize(_pOwner, _pTarget, _pEntityDesc)))
	{
		MSG_RETURN(nullptr, "CBrother_Behavior::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CBrother_Behavior::Clone(any)
{
	return make_private_shared_copy(CBrother_Behavior, *this);
}
  