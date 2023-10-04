#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_LeapBegin.h"

HRESULT CBrotherNodeLeaf_LeapBegin::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_LeapBegin::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));

	return S_OK;
}

void CBrotherNodeLeaf_LeapBegin::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::LEAP_BEGIN, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_LeapBegin::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::LEAP_BEGIN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::LEAP_BEGIN))
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

void CBrotherNodeLeaf_LeapBegin::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_LeapBegin> CBrotherNodeLeaf_LeapBegin::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_LeapBegin> pInstance = make_private_shared(CBrotherNodeLeaf_LeapBegin);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_LeapBegin::Create", "Failed to Initialize");
	}

	return pInstance;
}
