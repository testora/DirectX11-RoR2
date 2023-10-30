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

	Add_Child(CBrotherNodeLeaf_Death::Create(m_pBlackBoard));

	Add_Child(
		CBrotherNodeDecorator_Delay::Create(m_pBlackBoard, 1.f,
			CBrotherNodeLeaf_SpellChannel::Create(m_pBlackBoard)));

	Add_Child(CBrotherNodeParallel_HurtPrimaryShard::Create(m_pBlackBoard));

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SECONDARY:FISTSLAM")).value_or(nullptr),
			CBrotherNodeLeaf_FistSlam::Create(m_pBlackBoard)));

	Add_Child(
		CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 20.f, false,
			CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 4.f,
				CBrotherNodeLeaf_HurtWalk::Create(m_pBlackBoard))));

	Add_Child(
		CBrotherNodeDecorator_Delay::Create(m_pBlackBoard, 1.f, 
			CBrotherNodeLeaf_Stagger::Create(m_pBlackBoard)));

	Add_Child(
		CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 2.5f,
			CBrotherNodeLeaf_HurtIdle::Create(m_pBlackBoard)));

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
