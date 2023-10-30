#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_Smash.h"

HRESULT CBrotherNodeLeaf_Smash::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Smash::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Smash::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Smash::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Smash::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Smash::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CBrotherNodeLeaf_Smash::Activate()
{
	__super::Activate();

	m_bEffectAvailable = true;

	m_pTransform->LookAt(m_pTargetTransform);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SMASH_FORWARD, 1.5f, false, g_fDefaultInterpolationDuration, false);

	CGameInstance::Get_Instance()->Play_Sound(TEXT("brother_smash"), SOUND_CHANNEL::MONSTER_WEAPON,
		m_pTransform->Get_State(TRANSFORM::POSITION), m_pTargetTransform->Get_State(TRANSFORM::POSITION));
}

STATUS CBrotherNodeLeaf_Smash::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::SMASH_FORWARD))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::SMASH_FORWARD))
		{
			m_eStatus = STATUS::SUCCESS;
		}
		else
		{
			if (m_bEffectAvailable)
			{
				if (BROTHER_EFFECT_DELAY_PILLAR * 0.5f < m_fTimeAcc)
				{
					CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_EFFECT_PARTICLE_MESH_PILLAR)
						->Pop(m_pTransform->Get_State(TRANSFORM::POSITION) + Function::Rotate_By_Transform(m_pTransform, BROTHER_EFFECT_OFFSET_PILLAR));

					m_bEffectAvailable = false;
				}
			}

			m_eStatus = STATUS::RUNNING;
		}
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_Smash::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_Smash> CBrotherNodeLeaf_Smash::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_Smash> pInstance = make_private_shared(CBrotherNodeLeaf_Smash);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Smash::Create", "Failed to Initialize");
	}

	return pInstance;
}
