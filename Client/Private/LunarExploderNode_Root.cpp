#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNode_Root.h"

HRESULT CLunarExploderNode_Root::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNode_Root::Initialize", "Failed to __super::Initialize");
	}

	m_pBehaviorSelector = CLunarExploderNodeSelector_Main::Create(m_pBlackBoard);
	if (nullptr == m_pBehaviorSelector)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNode_Root::Initialize", "Failed to CBrotherNodeSelector_Phase::Create");
	}

	return S_OK;
}

void CLunarExploderNode_Root::Activate()
{
	__super::Activate();
}

STATUS CLunarExploderNode_Root::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = m_pBehaviorSelector->Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarExploderNode_Root::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarExploderNode_Root> CLunarExploderNode_Root::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarExploderNode_Root> pInstance = make_private_shared(CLunarExploderNode_Root);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNode_Root::Create", "Failed to Initialize");
	}

	return pInstance;
}
