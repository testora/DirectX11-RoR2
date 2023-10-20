#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNodeLeaf_TwinShot.h"

HRESULT CLunarGolemNodeLeaf_TwinShot::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_TwinShot::Initialize", "Failed to __super::Initialize");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_TwinShot::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_TwinShot::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_TwinShot::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_TwinShot::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_TwinShot::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CLunarGolemNodeLeaf_TwinShot::Activate()
{
	__super::Activate();

	m_iMagazine = 10;

	m_eAnimation = ANIMATION::LUNAR_GOLEM::FIRE_LEFT;
	m_pAnimator->Play_Animation(m_eAnimation, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CLunarGolemNodeLeaf_TwinShot::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	switch (m_eAnimation)
	{
	case ANIMATION::LUNAR_GOLEM::FIRE_LEFT:
	{
		if (m_pAnimator->Is_Playing(m_eAnimation))
		{
		//	if (m_pAnimator->Is_Finished(m_eAnimation))
			if (m_fTimeAcc > (11 - m_iMagazine) * 0.15f)
			{
				if (m_iMagazine)
				{
					--m_iMagazine;
					m_eAnimation = ANIMATION::LUNAR_GOLEM::FIRE_RIGHT;
					m_pAnimator->Play_Animation(m_eAnimation, 1.f, false, g_fDefaultInterpolationDuration, false);
					m_eStatus = STATUS::RUNNING;
				}
				else
				{
					m_eStatus = STATUS::SUCCESS;
				}
			}
			else
			{
				m_eStatus = STATUS::RUNNING;
			}
		}
		else
		{
			m_eStatus = STATUS::FAILURE;
		}
	}
	break;
	case ANIMATION::LUNAR_GOLEM::FIRE_RIGHT:
	{
		if (m_pAnimator->Is_Playing(m_eAnimation))
		{
		//	if (m_pAnimator->Is_Finished(m_eAnimation))
			if (m_fTimeAcc > (11 - m_iMagazine) * 0.15f)
			{
				if (m_iMagazine)
				{
					--m_iMagazine;
					m_eAnimation = ANIMATION::LUNAR_GOLEM::FIRE_LEFT;
					m_pAnimator->Play_Animation(m_eAnimation, 1.f, false, g_fDefaultInterpolationDuration, false);
					m_eStatus = STATUS::RUNNING;
				}
				else
				{
					m_eStatus = STATUS::SUCCESS;
				}
			}
			else
			{
				m_eStatus = STATUS::RUNNING;
			}
		}
		else
		{
			m_eStatus = STATUS::FAILURE;
		}
	}
	break;
	}

	return Return_Invoke();
}

void CLunarGolemNodeLeaf_TwinShot::Terminate()
{
	__super::Terminate();

	m_pAnimator->Set_Default(ANIMATION::LUNAR_GOLEM::IDLE);

	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::WALK_FORWARD))
	{
		m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::WALK_FORWARD);
	}
	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::WALK_BACKWARD))
	{
		m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::WALK_BACKWARD);
	}
	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::WALK_LEFT))
	{
		m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::WALK_LEFT);
	}
	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::WALK_RIGHT))
	{
		m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::WALK_RIGHT);
	}
}

shared_ptr<CLunarGolemNodeLeaf_TwinShot> CLunarGolemNodeLeaf_TwinShot::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeLeaf_TwinShot> pInstance = make_private_shared(CLunarGolemNodeLeaf_TwinShot);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeLeaf_TwinShot::Create", "Failed to Initialize");
	}

	return pInstance;
}
