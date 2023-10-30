#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNodeSelector_Main.h"

HRESULT CLunarGolemNodeSelector_Main::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeSelector_Main::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(CLunarGolemNodeLeaf_Death::Create(m_pBlackBoard));

	Add_Child(CLunarGolemNodeLeaf_LeapEnd::Create(m_pBlackBoard));

	Add_Child(
		CLunarGolemNodeDecorator_Range::Create(m_pBlackBoard, 40.f, true,
			CLunarGolemNodeParallel_WalkShot::Create(m_pBlackBoard)));

	Add_Child(
		CLunarGolemNodeDecorator_Range::Create(m_pBlackBoard, 80.f, true,
			CLunarGolemNodeLeaf_Sprint::Create(m_pBlackBoard, 1.f, 1.f, 1.f, 1.f)));

	Add_Child(CLunarGolemNodeLeaf_Sprint::Create(m_pBlackBoard, 1.f, 0.f, 0.f, 0.f));

	return S_OK;
}

void CLunarGolemNodeSelector_Main::Activate()
{
	__super::Activate();
}

STATUS CLunarGolemNodeSelector_Main::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarGolemNodeSelector_Main::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeSelector_Main> CLunarGolemNodeSelector_Main::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeSelector_Main> pInstance = make_private_shared(CLunarGolemNodeSelector_Main);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeSelector_Main::Create", "Failed to Initialize");
	}

	return pInstance;
}
