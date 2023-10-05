#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_Ult.h"

HRESULT CBrotherNodeLeaf_Ult::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Ult::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Ult::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CBrotherNodeLeaf_Ult::Activate()
{
	__super::Activate();

	m_eDirection = rand() % 2 ? DIRECTION::LEFT : DIRECTION::RIGHT;
}

STATUS CBrotherNodeLeaf_Ult::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	switch (m_eDirection)
	{
	case DIRECTION::LEFT:
	{

	}
	break;
	case DIRECTION::RIGHT:
	{

	}
	break;
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_Ult::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_Ult> CBrotherNodeLeaf_Ult::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_Ult> pInstance = make_private_shared(CBrotherNodeLeaf_Ult);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Ult::Create", "Failed to Initialize");
	}

	return pInstance;
}
