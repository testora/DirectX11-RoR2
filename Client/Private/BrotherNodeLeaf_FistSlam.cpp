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

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_FistSlam::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_FistSlam::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_FistSlam::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CBrotherNodeLeaf_FistSlam::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_FISTSLAM, 1.f, false, g_fDefaultInterpolationDuration, false);

	CGameInstance::Get_Instance()->Play_Sound(TEXT("Boss_MoonBrother_17"), SOUND_CHANNEL::MONSTER,
		m_pTransform->Get_State(TRANSFORM::POSITION), m_pTargetTransform->Get_State(TRANSFORM::POSITION));
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
