#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Phase2.h"

HRESULT CBrotherNodeSelector_Phase2::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase2::Initialize", "Failed to __super::Initialize");
	}

	m_pPhase = m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase2::Initialize", "Failed to Get: Owner:Phase");
	}

	return S_OK;
}

void CBrotherNodeSelector_Phase2::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Phase2::Invoke(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Hold('3'))
	{
		*m_pPhase = BROTHER_PHASE::PHASE3;
		m_eStatus = STATUS::FAILURE;
		return Return_Invoke();
	}

	Begin_Invoke(_fTimeDelta);

	m_eStatus = Function::InRange(*m_pPhase, BROTHER_PHASE::PHASE2, BROTHER_PHASE::MAX, "()") ?
	//	STATUS::FAILURE : __super::Invoke(_fTimeDelta);
		STATUS::FAILURE : STATUS::RUNNING;

	return Return_Invoke();
}

void CBrotherNodeSelector_Phase2::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_Phase2> CBrotherNodeSelector_Phase2::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Phase2> pInstance = make_private_shared(CBrotherNodeSelector_Phase2);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Phase2::Create", "Failed to Initialize");
	}

	return pInstance;
}
