#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_HurtShard.h"

HRESULT CBrotherNodeLeaf_HurtShard::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtShard::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtShard::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtShard::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtShard::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtShard::Initialize", "Failed to Get: Target:Transform");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtShard::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	return S_OK;
}

void CBrotherNodeLeaf_HurtShard::Activate()
{
	__super::Activate();

	m_bStart = false;

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_WALK_FORWARD, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_ENTER_FORWARD, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE, false);
}

STATUS CBrotherNodeLeaf_HurtShard::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::HURT_LUNARSHARD_ENTER_FORWARD))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::HURT_LUNARSHARD_ENTER_FORWARD))
		{
			m_bStart = true;
		}
	}

	if (m_bStart)
	{
		pair<ANIMATION::BROTHER, ANIMATION::BROTHER> pairAnimation;
		_float fRelativeAngle = Function::RelativeAngle(m_pTransform, m_pTargetTransform);

		if (Function::InRange(fRelativeAngle, -XM_PIDIV2, -XM_PI / 6.f))
		{
			pairAnimation = make_pair(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_LEFT, ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_LEFT);
		}
		else if (Function::InRange(fRelativeAngle, -XM_PI / 6.f, XM_PI / 6.f))
		{
			pairAnimation = make_pair(ANIMATION::BROTHER::HURT_LUNARSHARD_ENTER_FORWARD, ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_FORWARD);
		}
		else if (Function::InRange(fRelativeAngle, XM_PI / 6.f, XM_PIDIV2))
		{
			pairAnimation = make_pair(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_RIGHT, ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_RIGHT);
		}
		else
		{
			m_eStatus = STATUS::FAILURE;

			return Return_Invoke();
		}

		if (6.f > m_fTimeAcc)
		{
			m_pAnimator->Play_Animation(pairAnimation.first, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE);
		}
		else
		{
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_WALK_FORWARD, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE);
			m_pAnimator->Play_Animation(pairAnimation.second, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE);

			if (6.f + m_pAnimator->Get_Duration(pairAnimation.second) < m_fTimeAcc)
			{

				m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_WALK_FORWARD, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE);
				m_eStatus = STATUS::SUCCESS;
			}
		}
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_HurtShard::Terminate()
{
	__super::Terminate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_WALK_FORWARD, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE);
}

shared_ptr<CBrotherNodeLeaf_HurtShard> CBrotherNodeLeaf_HurtShard::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_HurtShard> pInstance = make_private_shared(CBrotherNodeLeaf_HurtShard);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_HurtShard::Create", "Failed to Initialize");
	}

	return pInstance;
}
