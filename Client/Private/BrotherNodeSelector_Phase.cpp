#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Phase.h"

HRESULT CBrotherNodeSelector_Phase::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase::Initialize", "Failed to Initialize");
	}

	m_pBlackBoard->Add_Anything(TEXT("Owner:Phase"), &m_ePhase);

	Add_Child(CBrotherNodeSelector_Phase0::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeSelector_Phase1::Create(m_pBlackBoard));

    return S_OK;
}

void CBrotherNodeSelector_Phase::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Phase::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus =__super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_Phase::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_Phase> CBrotherNodeSelector_Phase::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Phase> pInstance = make_private_shared(CBrotherNodeSelector_Phase);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Phase::Create", "Failed to Initialize");
	}

	return pInstance;
}
