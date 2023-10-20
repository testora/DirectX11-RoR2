#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"	
#include "BrotherNodeLeaf_SpellChannel.h"

HRESULT CBrotherNodeLeaf_SpellChannel::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SpellChannel::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SpellChannel::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_SpellChannel::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CBrotherNodeLeaf_SpellChannel::Activate()
{
	if (!m_bIsInitial)
	{
		m_eStatus = STATUS::FAILURE;
		return;
	}
	
	__super::Activate();

	m_pTransform->Set_State(TRANSFORM::POSITION, ARENA_CENTER);

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_SpellChannel::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER))
		{
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_LOOP);
		}
	}
	else
	{
		if (10.f <= m_fTimeAcc)
		{
			if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE))
			{
				if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE))
				{
					m_eStatus = STATUS::SUCCESS;
				}
			}
			else
			{
				m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE, 1.f, false, g_fDefaultInterpolationDuration, false);
			}
		}
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_SpellChannel::Terminate()
{
	__super::Terminate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_LOOP);

	m_bIsInitial = false;
}

shared_ptr<CBrotherNodeLeaf_SpellChannel> CBrotherNodeLeaf_SpellChannel::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_SpellChannel> pInstance = make_private_shared(CBrotherNodeLeaf_SpellChannel);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_SpellChannel::Create", "Failed to Initialize");
	}

	return pInstance;
}
