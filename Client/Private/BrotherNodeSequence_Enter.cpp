#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSequence_Enter.h"

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
}

STATUS CBrotherNodeSequence_Enter::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = m_bIsInital ? __super::Invoke(_fTimeDelta) : STATUS::FAILURE;

	return Return_Invoke();
}

void CBrotherNodeSequence_Enter::Terminate()
{
	__super::Terminate();

	m_bIsInital = false;
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
