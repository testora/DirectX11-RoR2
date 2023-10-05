#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSequence_Ult.h"

HRESULT CBrotherNodeSequence_Ult::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_Ult::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_Ult::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pSkillDesc = m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SPECIAL:ULT")).value_or(nullptr);
	if (nullptr == m_pSkillDesc)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_Ult::Initialize", "Failed to Get: Owner:Animator");
	}

	Add_Child(CBrotherNodeLeaf_Ult::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_Ult::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_Ult::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_Ult::Create(m_pBlackBoard));
	Add_Child(CBrotherNodeLeaf_Ult::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSequence_Ult::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::ULT_ENTER, 0.5f, true, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeSequence_Ult::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	
	if (15.f >= m_fTimeAcc)
	{
		if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::ULT_ENTER))
		{
			if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::ULT_ENTER))
			{
				m_pAnimator->Play_Animation(ANIMATION::BROTHER::ULT_CHANNEL, 0.5f, true, g_fDefaultInterpolationDuration, false);
			}
		}
	}
	else
	{
		if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::ULT_EXIT))
		{
			if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::ULT_EXIT))
			{
				m_eStatus = STATUS::SUCCESS;
				return Return_Invoke();
			}
		}
		else
		{
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::ULT_EXIT, 0.5f, true, g_fDefaultInterpolationDuration, false);
		}
	}

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSequence_Ult::Terminate()
{
	__super::Terminate();

	m_pSkillDesc->iMaxStock			= 0;
	m_pSkillDesc->iStock			= 0;
	m_pSkillDesc->fCurrentCoolTime	= 0.f;
}

shared_ptr<CBrotherNodeSequence_Ult> CBrotherNodeSequence_Ult::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSequence_Ult> pInstance = make_private_shared(CBrotherNodeSequence_Ult);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSequence_Ult::Create", "Failed to Initialize");
	}

	return pInstance;
}
