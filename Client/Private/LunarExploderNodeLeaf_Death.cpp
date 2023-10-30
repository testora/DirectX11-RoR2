#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeLeaf_Death.h"
#include "LunarExploder.h"

HRESULT CLunarExploderNodeLeaf_Death::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Death::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Death::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Death::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Death::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	return S_OK;
}

void CLunarExploderNodeLeaf_Death::Activate()
{
	__super::Activate();

	if (!m_pAnimator->Is_Playing(ANIMATION::LUNAR_EXPLODER::DEATH))
	{
		m_pAnimator->Set_Default(ANIMATION::LUNAR_EXPLODER::DEATH);
		m_pAnimator->Play_Animation(ANIMATION::LUNAR_EXPLODER::DEATH, 1.f, false, g_fDefaultInterpolationDuration, false);
	}
}

STATUS CLunarExploderNodeLeaf_Death::Invoke(_float _fTimeDelta)
{
	if (m_pEntityDesc->fHP >= 0.f)
	{
		return m_eStatus = STATUS::FAILURE;
	}

	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::LUNAR_EXPLODER::DEATH))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::LUNAR_EXPLODER::DEATH))
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, _float3(0.f, 0.f, 0.f));
		//	CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_MONSTER_LUNAREXPLODER)->Push(m_pBlackBoard->Get_Anything<shared_ptr<CLunarExploder>>(TEXT("Owner")).value_or(nullptr));
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
