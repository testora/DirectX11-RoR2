#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeParallel_HurtPrimaryShard.h"

CBrotherNodeParallel_HurtPrimaryShard::CBrotherNodeParallel_HurtPrimaryShard()
	: CParallel(POLICY::REQUIRE_ONE, POLICY::REQUIRE_ONE)
{
}

HRESULT CBrotherNodeParallel_HurtPrimaryShard::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeParallel_HurtPrimaryShard::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, []()->_bool { return true; },
			CBrotherNodeDecorator_Repeat::Create(m_pBlackBoard, 4.f,
				CBrotherNodeLeaf_HurtWalk::Create(m_pBlackBoard))));

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:SHARD:HURT")).value_or(nullptr),
			CBrotherNodeLeaf_HurtShard::Create(m_pBlackBoard)));

	return S_OK;
}

void CBrotherNodeParallel_HurtPrimaryShard::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeParallel_HurtPrimaryShard::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeParallel_HurtPrimaryShard::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeParallel_HurtPrimaryShard> CBrotherNodeParallel_HurtPrimaryShard::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeParallel_HurtPrimaryShard> pInstance = make_private_shared(CBrotherNodeParallel_HurtPrimaryShard);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeParallel_HurtPrimaryShard::Create", "Failed to Initialize");
	}

	return pInstance;
}
