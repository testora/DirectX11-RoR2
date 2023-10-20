#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeLeaf_Death.h"

HRESULT CLunarExploderNodeLeaf_Death::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Death::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Death::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CLunarExploderNodeLeaf_Death::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::IDLE_READY);
}

STATUS CLunarExploderNodeLeaf_Death::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_EXPLODER::DEATH))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::LUNAR_EXPLODER::DEATH))
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

void CLunarExploderNodeLeaf_Death::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarExploderNodeLeaf_Death> CLunarExploderNodeLeaf_Death::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarExploderNodeLeaf_Death> pInstance = make_private_shared(CLunarExploderNodeLeaf_Death);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeLeaf_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
