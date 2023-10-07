#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_HurtIdle.h"

HRESULT CBrotherNodeLeaf_HurtIdle::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtIdle::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtIdle::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CBrotherNodeLeaf_HurtIdle::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_LOOP);
}

STATUS CBrotherNodeLeaf_HurtIdle::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeLeaf_HurtIdle::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_HurtIdle> CBrotherNodeLeaf_HurtIdle::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_HurtIdle> pInstance = make_private_shared(CBrotherNodeLeaf_HurtIdle);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_HurtIdle::Create", "Failed to Initialize");
	}

	return pInstance;
}
