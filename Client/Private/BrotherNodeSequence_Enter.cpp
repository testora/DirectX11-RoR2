#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeSequence_Enter.h"
#include "BrotherNodeLeaf_LeapEnd.h"
#include "BrotherNodeLeaf_IdleReady.h"

HRESULT CBrotherNodeSequence_Enter::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_Appear::Initialize", "Failed to __super::Initialize");
	}
	
	Add_Child(CBrotherNodeLeaf_LeapEnd::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_IdleReady::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSequence_Enter::Activate()
{
	__super::Activate();

	m_fTimeAcc = 0.f;
}

STATUS CBrotherNodeSequence_Enter::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	if (!m_bIsArrived)
	{
		m_eStatus = __super::Invoke(_fTimeDelta);

		if (STATUS::SUCCESS == m_eStatus)
		{
			m_eStatus = STATUS::RUNNING;
			m_bIsArrived = true;
		}
	}
	else
	{
		m_fTimeAcc += _fTimeDelta;
		
		if (2.f < m_fTimeAcc)
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}

	return Return_Invoke();
}

void CBrotherNodeSequence_Enter::Terminate()
{
	__super::Terminate();

	*m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr) = BROTHER_PHASE::PHASE1;
}

shared_ptr<CBrotherNodeSequence_Enter> CBrotherNodeSequence_Enter::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSequence_Enter> pInstance = make_private_shared(CBrotherNodeSequence_Enter);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSequence_Enter::Create", "Failed to Initialize");
	}

	return pInstance;
}
