#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_SprintSmash.h"

HRESULT CBrotherNodeLeaf_SprintSmash::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SprintSmash::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SprintSmash::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SprintSmash::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SprintSmash::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SprintSmash::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CBrotherNodeLeaf_SprintSmash::Activate()
{
	__super::Activate();

	m_pTransform->LookAt(m_pTargetTransform);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_SMASH, 1.5f, false, g_fDefaultInterpolationDuration, false);
	m_pPhysics->Force(TRANSFORM::LOOK, BROTHER_SPRINT_SMASH_FORCE);
}

STATUS CBrotherNodeLeaf_SprintSmash::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::SPRINT_SMASH))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::SPRINT_SMASH))
		{
			m_eStatus = STATUS::SUCCESS;
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

	return Return_Invoke();
}

void CBrotherNodeLeaf_SprintSmash::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_SprintSmash> CBrotherNodeLeaf_SprintSmash::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_SprintSmash> pInstance = make_private_shared(CBrotherNodeLeaf_SprintSmash);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_SprintSmash::Create", "Failed to Initialize");
	}

	return pInstance;
}
