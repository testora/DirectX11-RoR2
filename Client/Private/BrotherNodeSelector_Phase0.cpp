#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Phase0.h"

HRESULT CBrotherNodeSelector_Phase0::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase0::Initialize", "Failed to Initialize");
	}

	m_pPhase = m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase0::Initialize", "Failed to Get: Owner:Phase");
	}

	Add_Child(CBrotherNodeDecorator_Delay::Create(m_pBlackBoard, 1.f,
		CBrotherNodeSequence_Enter::Create(m_pBlackBoard)));

	return S_OK;
}

void CBrotherNodeSelector_Phase0::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Phase0::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (BROTHER_PHASE::PHASE0 == *m_pPhase)
	{
		m_eStatus = __super::Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBrotherNodeSelector_Phase0::Terminate()
{
	__super::Terminate();

	*m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr) = BROTHER_PHASE::PHASE1;
}

shared_ptr<CBrotherNodeSelector_Phase0> CBrotherNodeSelector_Phase0::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Phase0> pInstance = make_private_shared(CBrotherNodeSelector_Phase0);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Phase0::Create", "Failed to Initialize");
	}

	return pInstance;
}
