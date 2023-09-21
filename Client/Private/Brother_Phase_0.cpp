#include "ClientPCH.h"
#include "Brother_Phase_0.h"
#include "GameInstance.h"

HRESULT CBrother_Phase_0::Initialize(shared_ptr<CGameObject> _pOwner, shared_ptr<CBrother_Behavior> _pBehavior, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pBehavior, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase_0::Initialize", "Failed to __super::Initialize");
	}

	m_eState = STATE::APPEAR;

	return S_OK;
}

void CBrother_Phase_0::Tick(_float _fTimeDelta)
{
	Handle_State(_fTimeDelta);

	m_fPhaseTimerAcc += _fTimeDelta;
}

void CBrother_Phase_0::Late_Tick(_float _fTimeDelta)
{
}

void CBrother_Phase_0::Handle_State(_float _fTimeDelta)
{
	m_fStateTimerAcc += _fTimeDelta;

	switch (m_eState)
	{
	case STATE::APPEAR:
	{
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));
		m_pAnimator->Play_Animation(ANIMATION::BROTHER::LEAP_END, 1.f, false, g_fDefaultInterpolationDuration, false);
		if (m_pAnimator->Is_Finished())
		{
			m_eState = STATE::READY;
			m_fStateTimerAcc = 0.f;
			return;
		}
	}
	break;
	case STATE::READY:
	{
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));
		m_pAnimator->Play_Animation(ANIMATION::BROTHER::IDLE_READY, 1.f, false, g_fDefaultInterpolationDuration, false);
		if (1.f < m_fStateTimerAcc)
		{
			m_eState = STATE::CHASE_TO_SMASH;
			m_fStateTimerAcc = 0.f;
			return;
		}
	}
	break;
	case STATE::CHASE:
	{
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));
		m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);
		m_pPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed, _fTimeDelta);
		if (Is_Target_InRange(BROTHER_CHASE_LIMIT_DISTANCE))
		{
			Dash();
			m_eState = STATE::DASH;
			m_fStateTimerAcc = 0.f;
			return;
		}
	}
	break;
	case STATE::CHASE_TO_SMASH:
	{
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));
		m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);
		m_pPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed, _fTimeDelta);
		if (Is_Target_InRange(BROTHER_SPRINT_SMASH_DISTANCE))
		{
			Sprint_Smash();
			m_eState = STATE::CHASE_SMASH;
			m_fStateTimerAcc = 0.f;
			return;
		}
	}
	break;
	case STATE::CHASE_SMASH:
	{
		if (1.5f < m_fStateTimerAcc)
		{
			Dash();
			m_eState = STATE::DASH;
			m_fStateTimerAcc = 0.f;
			return;
		}
	}
	break;
	case STATE::DASH:
	{
		if (m_pAnimator->Is_Finished())
		{
			Smash();
			m_eState = STATE::SMASH;
			m_fStateTimerAcc = 0.f;
			return;
		}
	}
	break;
	case STATE::SMASH:
	{
		if (m_pAnimator->Is_Finished())
		{
			m_eState = STATE::CHASE;
			m_fStateTimerAcc = 0.f;
			return;
		}
	}
	break;
	case STATE::MAX:
	break;
	}
}

shared_ptr<CBrother_Phase_0> CBrother_Phase_0::Create(shared_ptr<CGameObject> _pOwner, shared_ptr<CBrother_Behavior> _pBehavior, const ENTITYDESC* _pEntityDesc)
{
	shared_ptr<CBrother_Phase_0> pInstance = make_private_shared(CBrother_Phase_0);

	if (FAILED(pInstance->Initialize(_pOwner, _pBehavior, _pEntityDesc)))
	{
		MSG_RETURN(nullptr, "CBrother_Phase_0::Create", "Failed to Initialize");
	}

	return pInstance;
}
