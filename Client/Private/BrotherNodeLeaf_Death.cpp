#include "ClientPCH.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_Death.h"

HRESULT CBrotherNodeLeaf_Death::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Death::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Death::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Death::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	return S_OK;
}

void CBrotherNodeLeaf_Death::Activate()
{
	__super::Activate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_TO_DEATH, 1.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_Death::Invoke(_float _fTimeDelta)
{
	if (m_pEntityDesc->fHP >= 0.f)
	{
		return m_eStatus = STATUS::FAILURE;
	}

	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::HURT_TO_DEATH))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::HURT_TO_DEATH))
		{
			m_pAnimator->Set_Default(ANIMATION::BROTHER::DEATH_LOOP);
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::DEATH_LOOP);
			m_eStatus = STATUS::RUNNING;
		}
		else
		{
			m_eStatus = STATUS::RUNNING;
		}
	}
	else if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::DEATH_LOOP))
	{
		m_eStatus = STATUS::RUNNING;
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_Death::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_Death> CBrotherNodeLeaf_Death::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_Death> pInstance = make_private_shared(CBrotherNodeLeaf_Death);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
