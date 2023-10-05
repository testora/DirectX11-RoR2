#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSequence_DashSmash.h"

HRESULT CBrotherNodeSequence_DashSmash::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_DashSmash::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(CBrotherNodeLeaf_Dash::Create(m_pBlackBoard, 1.f, 0.f, 1.f, 1.f));
	Add_Child(CBrotherNodeLeaf_Smash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSequence_DashSmash::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSequence_DashSmash::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSequence_DashSmash::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSequence_DashSmash> CBrotherNodeSequence_DashSmash::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSequence_DashSmash> pInstance = make_private_shared(CBrotherNodeSequence_DashSmash);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSequence_DashSmash::Create", "Failed to Initialize");
	}

	return pInstance;
}
