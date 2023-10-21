#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Phase1.h"

HRESULT CBrotherNodeSelector_Phase1::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase1::Initialize", "Failed to Initialize");
	}
	
	m_pPhase = m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase1::Initialize", "Failed to Get: Owner:Phase");
	}

	Add_Child(CBrotherNodeLeaf_Shard::Create(m_pBlackBoard));
//	Add_Child(CBrotherNodeLeaf_LeapBegin::Create(m_pBlackBoard, m_pPhase));
//
//	Add_Child(
//		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SPECIAL:SHOCKWAVE")).value_or(nullptr),
//			CBrotherNodeSequence_Special::Create(m_pBlackBoard)));
//
//	Add_Child(CBrotherNodeSelector_Primary::Create(m_pBlackBoard));
//
//	Add_Child(
//		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SECONDARY")).value_or(nullptr),
//			CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 60.f, true,
//				CBrotherNodeSelector_Secondary::Create(m_pBlackBoard))));
//
//	Add_Child(
//		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:UTILITY")).value_or(nullptr),
//			CBrotherNodeLeaf_Dash::Create(m_pBlackBoard, 1.f, 0.f, 1.f, 1.f)));
//
//	Add_Child(
//		CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 40.f, false,
//			CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 1.f,
//				CBrotherNodeLeaf_Sprint::Create(m_pBlackBoard))));
//
//	Add_Child(
//		CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 20.f, false,
//			CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 1.f,
//				CBrotherNodeLeaf_Run::Create(m_pBlackBoard))));
//
//	Add_Child(CBrotherNodeLeaf_Dash::Create(m_pBlackBoard, 0.f, 1.f, 1.f, 1.f));
//
//	Add_Child(
//		CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 1.f,
//			CBrotherNodeLeaf_Run::Create(m_pBlackBoard)));

	return S_OK;
}

void CBrotherNodeSelector_Phase1::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Phase1::Invoke(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Hold('2'))
	{
		*m_pPhase = BROTHER_PHASE::MAX;
	}

	Begin_Invoke(_fTimeDelta);

	m_eStatus = Function::InRange(*m_pPhase, BROTHER_PHASE::PHASE1, BROTHER_PHASE::MAX, "()") ?
		STATUS::FAILURE : __super::Invoke(_fTimeDelta);

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
