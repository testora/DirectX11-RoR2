#include "ClientPCH.h"
#include "Brother_Phase.h"
#include "Brother_Behavior.h"
#include "GameInstance.h"

HRESULT CBrother_Phase::Initialize(shared_ptr<CGameObject> _pOwner, shared_ptr<CBrother_Behavior> _pBehavior, const ENTITYDESC* _pEntityDesc)
{
	m_pTransform = _pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Failed to Get_Component: TRANSFORM");
	}

	m_pModel = _pOwner->Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Failed to Get_Component: MODEL");
	}

	m_pPhysics = _pOwner->Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Failed to Get_Behavior: PHYSICS");
	}

	m_pAnimator = _pOwner->Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR);
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Failed to Get_Behavior: ANIMATOR");
	}

	if (nullptr == _pBehavior)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Invalid Parameter: CBrother_Behavior");
	}
	m_pBehavior = _pBehavior;

	m_pTarget = Function::Find_Player();
	if (nullptr == m_pTarget)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Failed to Find_Player");
	}

	m_pTargetTransform = m_pTarget->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Failed to Get_Component: TRANSFORM");
	}

	if (nullptr == _pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase::Initialize", "Invalid Parameter: ENTITYDESC");
	}
	m_pEntityDesc = _pEntityDesc;

	return S_OK;
}

HRESULT CBrother_Phase::Initialize(shared_ptr<CBrother_Phase> _pPrevious)
{
	m_pBehavior			= _pPrevious->m_pBehavior;

	m_pTransform		= _pPrevious->m_pTransform;
	m_pModel			= _pPrevious->m_pModel;
	m_pPhysics			= _pPrevious->m_pPhysics;
	m_pAnimator			= _pPrevious->m_pAnimator;

	m_pTarget			= _pPrevious->m_pTarget;
	m_pTargetTransform	= _pPrevious->m_pTargetTransform;

	m_pEntityDesc		= _pPrevious->m_pEntityDesc;

	return S_OK;
}

void CBrother_Phase::Change_Phase(BROTHER_PHASE _ePhase)
{
	if (shared_ptr<CBrother_Behavior> pBehavior = m_pBehavior.lock())
	{
		pBehavior->Change_Phase(_ePhase);
	}
	else
	{
		MSG_RETURN(, "CBrother_Phase::Change_Phase", "CBrother_Behavior Expired");
	}
}

void CBrother_Phase::Sprint_Smash()
{
	m_pPhysics->Flattern(true, false, true);
	m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_SMASH, 1.f, false, g_fDefaultInterpolationDuration, false);
	m_pPhysics->Force(TRANSFORM::LOOK, BROTHER_SPRINT_SMASH_FORCE);
}

void CBrother_Phase::Dash(DIRECTION _eDirection)
{
	switch (_eDirection)
	{
	case DIRECTION::FORWARD:
	{
		m_pPhysics->Flattern(true, false, true);
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));

		m_pAnimator->Play_Animation(ANIMATION::BROTHER::DASH_FORWARD, 1.f, false, g_fDefaultInterpolationDuration, false);
		m_pPhysics->Force(TRANSFORM::LOOK, BROTHER_DASH_FORCE);
	}
	break;
	case DIRECTION::BACKWARD:
	{
		m_pPhysics->Flattern(true, false, true);
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));

		m_pAnimator->Play_Animation(ANIMATION::BROTHER::DASH_BACKWARD, 1.f, false, g_fDefaultInterpolationDuration, false);
		m_pPhysics->Force(TRANSFORM::LOOK, -BROTHER_DASH_FORCE);
	}
	break;
	case DIRECTION::LEFT:
	{
		m_pPhysics->Flattern(true, false, true);
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));

		m_pAnimator->Play_Animation(ANIMATION::BROTHER::DASH_LEFT, 1.f, false, g_fDefaultInterpolationDuration, false);
		m_pPhysics->Force(TRANSFORM::RIGHT, -BROTHER_DASH_FORCE);
	}
	break;
	case DIRECTION::RIGHT:
	{
		m_pPhysics->Flattern(true, false, true);
		m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));

		m_pAnimator->Play_Animation(ANIMATION::BROTHER::DASH_RIGHT, 1.f, false, g_fDefaultInterpolationDuration, false);
		m_pPhysics->Force(TRANSFORM::RIGHT, BROTHER_DASH_FORCE);
	}
	break;
	case DIRECTION::MAX:
	{
		Dash(static_cast<DIRECTION>(rand() % static_cast<_uint>(DIRECTION::MAX)));
	}
	break;
	}
}

void CBrother_Phase::Smash()
{
	m_pPhysics->Flattern(true, false, true);
	m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SMASH_FORWARD, 3.f, false, g_fDefaultInterpolationDuration, false);
//	m_pAnimator->Reset();
}

_bool CBrother_Phase::Is_Target_InRange(_float _fRange) const
{
	if (shared_ptr<CBrother_Behavior> pBehavior = m_pBehavior.lock())
	{
		return pBehavior->Is_Target_InRange(_fRange);
	}
	else
	{
		MSG_RETURN(false, "CBrother_Phase::Is_Target_InRange", "CBrother_Behavior Expired");
	}
}
