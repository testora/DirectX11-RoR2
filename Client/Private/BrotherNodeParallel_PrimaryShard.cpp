#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeParallel_PrimaryShard.h"

CBrotherNodeParallel_PrimaryShard::CBrotherNodeParallel_PrimaryShard()
	: CParallel(POLICY::REQUIRE_ONE, POLICY::REQUIRE_ONE)
{
}

HRESULT CBrotherNodeParallel_PrimaryShard::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeParallel_PrimaryShard::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(CBrotherNodeLeaf_Sprint::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_Shard::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeParallel_PrimaryShard::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeParallel_PrimaryShard::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeParallel_PrimaryShard::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeParallel_PrimaryShard> CBrotherNodeParallel_PrimaryShard::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeParallel_PrimaryShard> pInstance = make_private_shared(CBrotherNodeParallel_PrimaryShard);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeParallel_PrimaryShard::Create", "Failed to Initialize");
	}

	return pInstance;
}
