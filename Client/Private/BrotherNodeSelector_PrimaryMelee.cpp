#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_PrimaryMelee.h"

HRESULT CBrotherNodeSelector_PrimaryMelee::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_PrimaryMelee::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:UTILITY")).value_or(nullptr), 
			CBrotherNodeSequence_DashSmash::Create(m_pBlackBoard)));
	Add_Child(CBrotherNodeLeaf_Smash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSelector_PrimaryMelee::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_PrimaryMelee::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_PrimaryMelee::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_PrimaryMelee> CBrotherNodeSelector_PrimaryMelee::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_PrimaryMelee> pInstance = make_private_shared(CBrotherNodeSelector_PrimaryMelee);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_PrimaryMelee::Create", "Failed to Initialize");
	}

	return pInstance;
}
