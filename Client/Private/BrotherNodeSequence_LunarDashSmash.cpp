#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSequence_LunarDashSmash.h"

HRESULT CBrotherNodeSequence_LunarDashSmash::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_LunarDashSmash::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(CBrotherNodeLeaf_Dash::Create(m_pBlackBoard, 1.f, 0.f, 1.f, 1.f));
	Add_Child(CBrotherNodeLeaf_LunarSmash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSequence_LunarDashSmash::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSequence_LunarDashSmash::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSequence_LunarDashSmash::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSequence_LunarDashSmash> CBrotherNodeSequence_LunarDashSmash::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSequence_LunarDashSmash> pInstance = make_private_shared(CBrotherNodeSequence_LunarDashSmash);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSequence_LunarDashSmash::Create", "Failed to Initialize");
	}

	return pInstance;
}
