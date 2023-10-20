#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeLeaf_Idle.h"

HRESULT CLunarExploderNodeLeaf_Idle::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Idle::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Idle::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CLunarExploderNodeLeaf_Idle::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::LUNAR_EXPLODER::IDLE);
}

STATUS CLunarExploderNodeLeaf_Idle::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarExploderNodeLeaf_Idle::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarExploderNodeLeaf_Idle> CLunarExploderNodeLeaf_Idle::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarExploderNodeLeaf_Idle> pInstance = make_private_shared(CLunarExploderNodeLeaf_Idle);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeLeaf_Idle::Create", "Failed to Initialize");
	}

	return pInstance;
}
