#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_LunarPrimary.h"

HRESULT CBrotherNodeSelector_LunarPrimary::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_LunarPrimary::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:SHARD")).value_or(nullptr),
			CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 80.f, false,
				CBrotherNodeParallel_PrimaryShard::Create(m_pBlackBoard))));
	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:MELEE:LUNAR")).value_or(nullptr),
			CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 40.f, true,
				CBrotherNodeSelector_LunarPrimaryMelee::Create(m_pBlackBoard))));

	return S_OK;
}

void CBrotherNodeSelector_LunarPrimary::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_LunarPrimary::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_LunarPrimary::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_LunarPrimary> CBrotherNodeSelector_LunarPrimary::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_LunarPrimary> pInstance = make_private_shared(CBrotherNodeSelector_LunarPrimary);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_LunarPrimary::Create", "Failed to Initialize");
	}

	return pInstance;
}
