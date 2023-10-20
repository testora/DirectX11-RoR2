#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolemNodeLeaf_LeapEnd.h"

HRESULT CLunarGolemNodeLeaf_LeapEnd::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_LeapEnd::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_LeapEnd::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CLunarGolemNodeLeaf_LeapEnd::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::LEAP_BEGIN);
}

STATUS CLunarGolemNodeLeaf_LeapEnd::Invoke(_float _fTimeDelta)
{
	if (!m_bIsInitial)
	{
		return STATUS::FAILURE;
	}

	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::LEAP_BEGIN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::LUNAR_GOLEM::LEAP_BEGIN))
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

void CLunarGolemNodeLeaf_LeapEnd::Terminate()
{
	__super::Terminate();

	m_bIsInitial = false;
}

shared_ptr<CLunarGolemNodeLeaf_LeapEnd> CLunarGolemNodeLeaf_LeapEnd::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeLeaf_LeapEnd> pInstance = make_private_shared(CLunarGolemNodeLeaf_LeapEnd);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeLeaf_LeapEnd::Create", "Failed to Initialize");
	}

	return pInstance;
}
