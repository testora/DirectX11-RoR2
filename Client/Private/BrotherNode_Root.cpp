#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNode_Root.h"
#include "BrotherNodeSelector_Phase.h"

HRESULT CBrotherNode_Root::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to __super::Initialize");
	}

	m_pPhaseSelector = CBrotherNodeSelector_Phase::Create(m_pBlackBoard);

	return S_OK;
}

void CBrotherNode_Root::Activate()
{
	__super::Activate();
}

STATUS CBrotherNode_Root::Invoke(_float _fTimeDelta)
{
	return m_pPhaseSelector->Invoke(_fTimeDelta);
}

void CBrotherNode_Root::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNode_Root> CBrotherNode_Root::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNode_Root> pInstance = make_private_shared(CBrotherNode_Root);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNode_Root::Create", "Failed to Initialize");
	}

	return pInstance;
}
