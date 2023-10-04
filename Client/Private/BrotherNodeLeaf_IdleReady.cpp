#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_IdleReady.h"

HRESULT CBrotherNodeLeaf_IdleReady::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_IdleReady::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));

	return S_OK;
}

void CBrotherNodeLeaf_IdleReady::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::IDLE_READY);
}

STATUS CBrotherNodeLeaf_IdleReady::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::IDLE_READY))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::IDLE_READY))
		{
			m_eStatus = STATUS::SUCCESS;
		}
		else
		{
			m_eStatus = STATUS::RUNNING;
		}
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_IdleReady::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_IdleReady> CBrotherNodeLeaf_IdleReady::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_IdleReady> pInstance = make_private_shared(CBrotherNodeLeaf_IdleReady);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_IdleReady::Create", "Failed to Initialize");
	}

	return pInstance;
}
