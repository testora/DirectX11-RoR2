#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_LeapEnd.h"

HRESULT CBrotherNodeLeaf_LeapEnd::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_LeapEnd::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_LeapEnd::Initialize", "Failed to Get: Owner:Transform");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_LeapEnd::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CBrotherNodeLeaf_LeapEnd::Activate()
{
	__super::Activate();

	m_pTransform->Set_State(TRANSFORM::POSITION, ARENA_CENTER);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::LEAP_END, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_LeapEnd::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::LEAP_END))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::LEAP_END))
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

void CBrotherNodeLeaf_LeapEnd::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_LeapEnd> CBrotherNodeLeaf_LeapEnd::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_LeapEnd> pInstance = make_private_shared(CBrotherNodeLeaf_LeapEnd);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_LeapEnd::Create", "Failed to Initialize");
	}

	return pInstance;
}
