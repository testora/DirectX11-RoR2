#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_FistSlam.h"

HRESULT CBrotherNodeLeaf_FistSlam::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_FistSlam::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_FistSlam::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CBrotherNodeLeaf_FistSlam::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_FISTSLAM, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_FistSlam::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (5.f <= m_fTimeAcc)
	{
		m_eStatus = STATUS::SUCCESS;
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_FistSlam::Terminate()
{
	__super::Terminate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_LOOP);
}

shared_ptr<CBrotherNodeLeaf_FistSlam> CBrotherNodeLeaf_FistSlam::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_FistSlam> pInstance = make_private_shared(CBrotherNodeLeaf_FistSlam);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_FistSlam::Create", "Failed to Initialize");
	}

	return pInstance;
}
