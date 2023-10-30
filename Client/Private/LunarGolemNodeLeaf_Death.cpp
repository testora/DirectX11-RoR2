#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolemNodeLeaf_Death.h"
#include "LunarGolem.h"

HRESULT CLunarGolemNodeLeaf_Death::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Death::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Death::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Death::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Death::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	return S_OK;
}

void CLunarGolemNodeLeaf_Death::Activate()
{
	__super::Activate();

	if (!m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::DEATH2))
	{
		m_pAnimator->Set_Default(ANIMATION::LUNAR_GOLEM::DEATH2);
		m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::DEATH2, 1.f, false, g_fDefaultInterpolationDuration, false);
	}
}

STATUS CLunarGolemNodeLeaf_Death::Invoke(_float _fTimeDelta)
{
	if (m_pEntityDesc->fHP >= 0.f)
	{
		return m_eStatus = STATUS::FAILURE;
	}

	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_GOLEM::DEATH2))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::LUNAR_GOLEM::DEATH2))
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, _float3(0.f, 0.f, 0.f));
			m_pAnimator->Lock();
			m_eStatus = STATUS::RUNNING;
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

void CLunarGolemNodeLeaf_Death::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeLeaf_Death> CLunarGolemNodeLeaf_Death::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeLeaf_Death> pInstance = make_private_shared(CLunarGolemNodeLeaf_Death);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeLeaf_Death::Create", "Failed to Initialize");
	}

	return pInstance;
}
