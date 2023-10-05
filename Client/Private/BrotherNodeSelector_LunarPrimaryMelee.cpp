#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_LunarPrimaryMelee.h"

HRESULT CBrotherNodeSelector_LunarPrimaryMelee::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_LunarPrimaryMelee::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:UTILITY")).value_or(nullptr),
			CBrotherNodeSequence_LunarDashSmash::Create(m_pBlackBoard)));
	Add_Child(CBrotherNodeLeaf_LunarSmash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSelector_LunarPrimaryMelee::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_LunarPrimaryMelee::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_LunarPrimaryMelee::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_LunarPrimaryMelee> CBrotherNodeSelector_LunarPrimaryMelee::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_LunarPrimaryMelee> pInstance = make_private_shared(CBrotherNodeSelector_LunarPrimaryMelee);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_LunarPrimaryMelee::Create", "Failed to Initialize");
	}

	return pInstance;
}
