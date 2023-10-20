#include "ClientPCH.h"
#include "Golem_Behavior.h"
#include "Golem.h"
#include "Golem_Untarget.h"
#include "Golem_Target.h"
#include "Golem_Hit.h"
#include "GameInstance.h"

HRESULT CGolem_Behavior::Initialize(shared_ptr<CGameObject> _pOwner, shared_ptr<CGameObject> _pTarget, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pTarget, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CGolem_Behavior::Initialize", "Failed to __super::Initialize");
	}

	m_pGolem = dynamic_pointer_cast<CGolem>(_pOwner);
	if (nullptr == m_pGolem)
	{
		MSG_RETURN(E_FAIL, "CGolem_Behavior::Initialize", "Failed to Cast");
	}

	m_pPhysics = _pOwner->Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CGolem_Behavior::Initialize", "Failed to Get_Behavior: PHYSICS");
	}

	m_pAnimator = _pOwner->Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR);
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CGolem_Behavior::Initialize", "Failed to Get_Behavior: ANIMATOR");
	}

	if (FAILED(Ready_State()))
	{
		MSG_RETURN(E_FAIL, "CGolem_Behavior::Initialize", "Failed to Ready_State");
	}

	return S_OK;
}

void CGolem_Behavior::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	Manage_State();
	Handle_State(_fTimeDelta);

	m_umapState[m_eState]->Tick(_fTimeDelta);
}

void CGolem_Behavior::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_umapState[m_eState]->Late_Tick(_fTimeDelta);
}

HRESULT CGolem_Behavior::Ready_State()
{
	m_umapState[STATE::UNTARGET]	= CGolem_Untarget::Create(m_pGolem);
	m_umapState[STATE::TARGET]		= CGolem_Target::Create(m_pGolem);
	m_umapState[STATE::HIT]			= CGolem_Hit::Create(m_pGolem);

	return S_OK;
}

void CGolem_Behavior::Set_State(STATE _eState)
{
	if (m_eState != _eState)
	{
		m_fTimeAcc = 0.f;
	}
	m_eState = _eState;
}

void CGolem_Behavior::Manage_State()
{
	if (m_eState == STATE::TARGET || m_eState == STATE::HIT)
	{
		return;
	}

	if (Is_Target_InRange(20.f))
	{
		Set_State(STATE::TARGET);
	}
	else
	{
		Set_State(STATE::UNTARGET);
	}
}

void CGolem_Behavior::Handle_State(_float _fTimeDelta)
{
//	if (m_pAnimator->Current_Animation<ANIMATION::GOLEM>() == ANIMATION::GOLEM::HIT_LIGHTIMPACT)
	{
	//	if (!m_pAnimator->Is_Finished())
			return;
	}

	switch (m_eState)
	{
	case STATE::UNTARGET:
	{
		m_fTimeAcc += _fTimeDelta;
		if (m_fTimeAcc >= GOLEM_MOVEMENT_TIME_MAX)
		{
		//	m_pGolem->Wander();
			m_fTimeAcc = 0.f;
		}
	}
	break;
	case STATE::TARGET:
	{
		Chase_Target(_fTimeDelta);
		if (_float3(m_pTargetTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION)).length() >= 5.f)
		{
			m_pAnimator->Play_Animation(ANIMATION::GOLEM::WALK, 1.5f);
		}
		else
		{
			m_fAtkAcc += _fTimeDelta;
			if (m_fAtkAcc > 3.f)
			{
				m_pAnimator->Play_Animation(ANIMATION::GOLEM::ATTACK_SMACK, 1.5f, false, g_fDefaultInterpolationDuration, false);
			//	if(m_pAnimator->Is_Finished())
				{
				//	m_pAnimator->Reset();
					m_fAtkAcc = 0.f;
				}
			}
			else
			{
				m_pAnimator->Play_Animation(ANIMATION::GOLEM::IDLE, 1.5f);
			}
		}
	}
	break;
	case STATE::HIT:
	{
		m_pTarget = Function::Find_Player();
	//	if (m_pAnimator->Is_Finished())
		{
			Set_State(STATE::TARGET);
		}
	}
	break;
	}
}

shared_ptr<CGolem_Behavior> CGolem_Behavior::Create(shared_ptr<CGameObject> _pOwner, shared_ptr<CGameObject> _pTarget, const ENTITYDESC* _pEntityDesc)
{
	shared_ptr<CGolem_Behavior> pInstance = make_private_shared(CGolem_Behavior);

	if (FAILED(pInstance->Initialize(_pOwner, _pTarget, _pEntityDesc)))
	{
		MSG_RETURN(nullptr, "CGolem_Behavior::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CGolem_Behavior::Clone(any)
{
	return make_private_shared_copy(CGolem_Behavior, *this);
}
