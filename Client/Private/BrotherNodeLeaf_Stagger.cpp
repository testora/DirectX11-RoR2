#include "ClientPCH.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_Stagger.h"

HRESULT CBrotherNodeLeaf_Stagger::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Stagger::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Stagger::Initialize", "Failed to Get: Owner:Animator");
	}

	return S_OK;
}

void CBrotherNodeLeaf_Stagger::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_STAGGER_ENTER, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_Stagger::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::HURT_STAGGER_ENTER))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::HURT_STAGGER_ENTER))
		{
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_STAGGER_LOOP);
		}
	}
	else
	{
		if (2.f <= m_fTimeAcc)
		{
			if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::HURT_STRGGER_EXIT))
			{
				if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::HURT_STRGGER_EXIT))
				{
					m_eStatus = STATUS::SUCCESS;
				}
			}
			else
			{
				m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_STRGGER_EXIT, 1.f, false, g_fDefaultInterpolationDuration, false);
			}
		}
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_Stagger::Terminate()
{
	__super::Terminate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_LOOP);
}

shared_ptr<CBrotherNodeLeaf_Stagger> CBrotherNodeLeaf_Stagger::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_Stagger> pInstance = make_private_shared(CBrotherNodeLeaf_Stagger);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Stagger::Create", "Failed to Initialize");
	}

	return pInstance;
}
