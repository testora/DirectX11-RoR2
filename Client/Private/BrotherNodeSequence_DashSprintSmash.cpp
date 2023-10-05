#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSequence_DashSprintSmash.h"

HRESULT CBrotherNodeSequence_DashSprintSmash::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_DashSprintSmash::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(CBrotherNodeLeaf_DashSide::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_SprintSmash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSequence_DashSprintSmash::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSequence_DashSprintSmash::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSequence_DashSprintSmash::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSequence_DashSprintSmash> CBrotherNodeSequence_DashSprintSmash::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSequence_DashSprintSmash> pInstance = make_private_shared(CBrotherNodeSequence_DashSprintSmash);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSequence_DashSprintSmash::Create", "Failed to Initialize");
	}

	return pInstance;
}
