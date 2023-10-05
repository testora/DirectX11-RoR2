#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_DashSide.h"

HRESULT CBrotherNodeLeaf_DashSide::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_DashSprintSmash::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_DashSprintSmash::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_DashSprintSmash::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_DashSprintSmash::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_DashSprintSmash::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CBrotherNodeLeaf_DashSide::Activate()
{
	__super::Activate();

	m_pPhysics->Flattern(true, false, true);
	m_pTransform->LookAt(m_pTargetTransform);

	_bool	bDirection	= rand() % 2;
	_float	fDistance	= Function::Distance(m_pTransform, m_pTargetTransform);
	_float	fTheta		= acosf(Function::Clamp(-1.f, 1.f, fDistance / 40));

	m_pTransform->Rotate(TRANSFORM::UP, (bDirection ? 1.f : -1.f) * fTheta - XM_PIDIV2);
	m_pPhysics->Force(TRANSFORM::RIGHT, (bDirection ? 1.f : -1.f) * BROTHER_DASH_FORCE);
	m_pAnimator->Play_Animation(bDirection ? ANIMATION::BROTHER::DASH_RIGHT : ANIMATION::BROTHER::DASH_LEFT, 2.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_DashSide::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::DASH_LEFT) || m_pAnimator->Is_Playing(ANIMATION::BROTHER::DASH_RIGHT))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::DASH_LEFT) || m_pAnimator->Is_Finished(ANIMATION::BROTHER::DASH_RIGHT))
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

void CBrotherNodeLeaf_DashSide::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_DashSide> CBrotherNodeLeaf_DashSide::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_DashSide> pInstance = make_private_shared(CBrotherNodeLeaf_DashSide);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_DashSprintSmash::Create", "Failed to Initialize");
	}

	return pInstance;
}
