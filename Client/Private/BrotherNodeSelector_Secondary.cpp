#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_Secondary.h"

HRESULT CBrotherNodeSelector_Secondary::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_Secondary::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:UTILITY")).value_or(nullptr),
			CBrotherNodeSequence_DashSprintSmash::Create(m_pBlackBoard)));
	Add_Child(CBrotherNodeLeaf_SprintSmash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSelector_Secondary::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_Secondary::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_Secondary::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_Secondary> CBrotherNodeSelector_Secondary::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_Secondary> pInstance = make_private_shared(CBrotherNodeSelector_Secondary);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_Secondary::Create", "Failed to Initialize");
	}

	return pInstance;
}
