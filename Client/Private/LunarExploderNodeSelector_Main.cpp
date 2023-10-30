#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeSelector_Main.h"

HRESULT CLunarExploderNodeSelector_Main::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeSelector_Main::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(CLunarExploderNodeLeaf_Death::Create(m_pBlackBoard));
	Add_Child(CLunarExploderNodeLeaf_Spawn::Create(m_pBlackBoard));

	Add_Child(
		CLunarExploderNodeDecorator_Range::Create(m_pBlackBoard, 30.f, true,
			CLunarExploderNodeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
				CLunarExploderNodeLeaf_Sprint::Create(m_pBlackBoard, 1.f, 1.f, 1.f))));

	Add_Child(
		CLunarExploderNodeDecorator_Repeat::Create(m_pBlackBoard, 1.5f,
			CLunarExploderNodeLeaf_Sprint::Create(m_pBlackBoard, 1.f, 0.f, 0.f)));

	return S_OK;
}

void CLunarExploderNodeSelector_Main::Activate()
{
	__super::Activate();
}

STATUS CLunarExploderNodeSelector_Main::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarExploderNodeSelector_Main::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarExploderNodeSelector_Main> CLunarExploderNodeSelector_Main::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarExploderNodeSelector_Main> pInstance = make_private_shared(CLunarExploderNodeSelector_Main);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeSelector_Main::Create", "Failed to Initialize");
	}

	return pInstance;
}
