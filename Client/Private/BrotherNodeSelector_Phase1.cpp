#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeSelector_Phase1.h"
#include "BrotherNodeSequence_SprintSmash.h"

HRESULT CBrotherNodeSelector_Phase1::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase1::Initialize", "Failed to Initialize");
	}
	
	m_pPhase = m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);

	Add_Child(CBrotherNodeSequence_SprintSmash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSelector_Phase1::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Phase1::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	if (BROTHER_PHASE::PHASE1 == *m_pPhase)
	{
		m_eStatus = __super::Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBrotherNodeSelector_Phase1::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_Phase1> CBrotherNodeSelector_Phase1::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Phase1> pInstance = make_private_shared(CBrotherNodeSelector_Phase1);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Phase1::Create", "Failed to Initialize");
	}

	return pInstance;
}
