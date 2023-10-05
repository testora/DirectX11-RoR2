#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Phase3.h"

HRESULT CBrotherNodeSelector_Phase3::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase3::Initialize", "Failed to __super::Initialize");
	}

	m_pPhase = m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Phase3::Initialize", "Failed to Get: Owner:Phase");
	}

	Add_Child(CBrotherNodeSequence_Enter::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_LeapBegin::Create(m_pBlackBoard, m_pPhase));

	Add_Child(CBrotherNodeSelector_LunarSpecial::Create(m_pBlackBoard));

	Add_Child(CBrotherNodeSelector_LunarPrimary::Create(m_pBlackBoard));

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SECONDARY")).value_or(nullptr),
			CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 60.f, true,
				CBrotherNodeSelector_Secondary::Create(m_pBlackBoard))));

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:UTILITY")).value_or(nullptr),
			CBrotherNodeLeaf_Dash::Create(m_pBlackBoard, 1.f, 0.f, 1.f, 1.f)));

	Add_Child(
		CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 40.f, false,
			CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 1.f,
				CBrotherNodeLeaf_Sprint::Create(m_pBlackBoard))));

	Add_Child(
		CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 20.f, false,
			CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 1.f,
				CBrotherNodeLeaf_Run::Create(m_pBlackBoard))));

	Add_Child(CBrotherNodeLeaf_Dash::Create(m_pBlackBoard, 0.f, 1.f, 1.f, 1.f));

	Add_Child(
		CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, nullptr,
			CBrotherNodeLeaf_Run::Create(m_pBlackBoard)));

	return S_OK;
}

void CBrotherNodeSelector_Phase3::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Phase3::Invoke(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Hold('4'))
	{
		*m_pPhase = BROTHER_PHASE::MAX;
	}

	Begin_Invoke(_fTimeDelta);

	m_eStatus = Function::InRange(*m_pPhase, BROTHER_PHASE::PHASE3, BROTHER_PHASE::MAX, "()") ?
		STATUS::FAILURE : __super::Invoke(_fTimeDelta);

	if (m_eStatus == STATUS::FAILURE)
		int a = 1;

	return Return_Invoke();
}

void CBrotherNodeSelector_Phase3::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_Phase3> CBrotherNodeSelector_Phase3::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Phase3> pInstance = make_private_shared(CBrotherNodeSelector_Phase3);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Phase3::Create", "Failed to Initialize");
	}

	return pInstance;
}
