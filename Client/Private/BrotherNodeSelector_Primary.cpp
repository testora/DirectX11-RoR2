#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Primary.h"

HRESULT CBrotherNodeSelector_Primary::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Primary::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:SHARD")).value_or(nullptr),
			CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 140.f, false,
				CBrotherNodeParallel_PrimaryShard::Create(m_pBlackBoard))));
	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:MELEE")).value_or(nullptr),
			CBrotherNodeDecorator_Range::Create(m_pBlackBoard, 70.f, true,
				CBrotherNodeSelector_PrimaryMelee::Create(m_pBlackBoard))));

	return S_OK;
}

void CBrotherNodeSelector_Primary::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Primary::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_Primary::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_Primary> CBrotherNodeSelector_Primary::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Primary> pInstance = make_private_shared(CBrotherNodeSelector_Primary);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Primary::Create", "Failed to Initialize");
	}

	return pInstance;
}
