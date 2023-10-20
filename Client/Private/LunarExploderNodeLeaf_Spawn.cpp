#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeLeaf_Spawn.h"

HRESULT CLunarExploderNodeLeaf_Spawn::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Spawn::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Spawn::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CLunarExploderNodeLeaf_Spawn::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::LUNAR_EXPLODER::SPAWN, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CLunarExploderNodeLeaf_Spawn::Invoke(_float _fTimeDelta)
{
	if (!m_bIsInitial)
	{
		return STATUS::FAILURE;
	}

	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_EXPLODER::SPAWN))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::LUNAR_EXPLODER::SPAWN))
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

void CLunarExploderNodeLeaf_Spawn::Terminate()
{
	__super::Terminate();

	m_bIsInitial = false;
}

shared_ptr<CLunarExploderNodeLeaf_Spawn> CLunarExploderNodeLeaf_Spawn::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarExploderNodeLeaf_Spawn> pInstance = make_private_shared(CLunarExploderNodeLeaf_Spawn);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeLeaf_Spawn::Create", "Failed to Initialize");
	}

	return pInstance;
}
