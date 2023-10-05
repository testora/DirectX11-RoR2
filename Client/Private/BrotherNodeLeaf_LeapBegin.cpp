#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_LeapBegin.h"
#include "Brother.h"

CBrotherNodeLeaf_LeapBegin::CBrotherNodeLeaf_LeapBegin(BROTHER_PHASE* _pPhase)
	: m_pPhase(_pPhase)
	, m_ePhase(_pPhase ? *_pPhase : BROTHER_PHASE::MAX)
{
}

HRESULT CBrotherNodeLeaf_LeapBegin::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_LeapBegin::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_LeapBegin::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pBrother = m_pBlackBoard->Get_Anything<shared_ptr<CBrother>>(TEXT("Owner")).value_or(nullptr);
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_LeapBegin::Initialize", "Failed to Get: Owner");
	}

	return S_OK;
}

void CBrotherNodeLeaf_LeapBegin::Activate()
{
	if (BROTHER_PHASE::MAX != m_ePhase && BROTHER_PHASE::MAX != *m_pPhase)
	{
		return;
	}

	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::LEAP_BEGIN, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_LeapBegin::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::LEAP_BEGIN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::LEAP_BEGIN))
		{
			m_pBrother->Set_Render(false);
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

	if (BROTHER_PHASE::MAX != m_ePhase && BROTHER_PHASE::MAX == *m_pPhase)
	{
		m_pBrother->Set_Render(false);
		*m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr) = static_cast<BROTHER_PHASE>(static_cast<_uint>(m_ePhase) + 1);
	}
}

shared_ptr<CBrotherNodeLeaf_LeapBegin> CBrotherNodeLeaf_LeapBegin::Create(shared_ptr<CBlackBoard> _pBlackBoard, BROTHER_PHASE* _pPhase)
{
	shared_ptr<CBrotherNodeLeaf_LeapBegin> pInstance = make_private_shared(CBrotherNodeLeaf_LeapBegin, _pPhase);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_LeapBegin::Create", "Failed to Initialize");
	}

	return pInstance;
}
