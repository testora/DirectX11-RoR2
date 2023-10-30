#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolemNodeLeaf_LeapEnd.h"

HRESULT CLunarGolemNodeLeaf_LeapEnd::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_LeapEnd::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_LeapEnd::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_LeapEnd::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_LeapEnd::Initialize", "Failed to Get: Target:Animator");
	}

	return S_OK;
}

void CLunarGolemNodeLeaf_LeapEnd::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::FINISH_LEAP, 1.f, false, g_fDefaultInterpolationDuration, false);

	CGameInstance::Get_Instance()->Play_Sound(TEXT("Mob_lunarGolem_15"), SOUND_CHANNEL::MONSTER,
		m_pTransform->Get_State(TRANSFORM::POSITION), m_pTargetTransform->Get_State(TRANSFORM::POSITION));
}

STATUS CLunarGolemNodeLeaf_LeapEnd::Invoke(_float _fTimeDelta)
{
	if (!m_bIsInitial)
	{
		return STATUS::FAILURE;
	}

	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::FINISH_LEAP))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::LUNAR_GOLEM::FINISH_LEAP))
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

void CLunarGolemNodeLeaf_LeapEnd::Terminate()
{
	__super::Terminate();

	m_bIsInitial = false;
}

shared_ptr<CLunarGolemNodeLeaf_LeapEnd> CLunarGolemNodeLeaf_LeapEnd::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeLeaf_LeapEnd> pInstance = make_private_shared(CLunarGolemNodeLeaf_LeapEnd);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeLeaf_LeapEnd::Create", "Failed to Initialize");
	}

	return pInstance;
}