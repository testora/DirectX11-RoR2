#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNodeLeaf_Idle.h"

HRESULT CLunarGolemNodeLeaf_Idle::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Idle::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Idle::Initialize", "Failed to Get: Owner:Animator");
	}
	
	return S_OK;
}

void CLunarGolemNodeLeaf_Idle::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::IDLE);
}

STATUS CLunarGolemNodeLeaf_Idle::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarGolemNodeLeaf_Idle::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeLeaf_Idle> CLunarGolemNodeLeaf_Idle::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeLeaf_Idle> pInstance = make_private_shared(CLunarGolemNodeLeaf_Idle);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeLeaf_Idle::Create", "Failed to Initialize");
	}

	return pInstance;
}
