#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Phase4.h"

HRESULT CBrotherNodeSelector_Phase4::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase4::Initialize", "Failed to __super::Initialize");
	}

	m_pPhase = m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase4::Initialize", "Failed to Get: Owner:Phase");
	}

	return S_OK;
}

void CBrotherNodeSelector_Phase4::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Phase4::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_Phase4::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_Phase4> CBrotherNodeSelector_Phase4::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Phase4> pInstance = make_private_shared(CBrotherNodeSelector_Phase4);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Phase4::Create", "Failed to Initialize");
	}

	return pInstance;
}
