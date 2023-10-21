#include "ClientPCH.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_Shard.h"
#include "Bone.h"

HRESULT CBrotherNodeLeaf_Shard::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Shard::Initialize", "Failed to __super::Initialize");
	}

	m_pPool = CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), POOL_MONSTER_BROTHER_LUNARSHARD);
	if (nullptr == m_pPool)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Shard::Initialize", "Failed to Find_Pool");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Shard::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pModel = m_pBlackBoard->Get_System<CModel>(TEXT("Owner:Model"));
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Shard::Initialize", "Failed to Get: Owner:Model");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Shard::Initialize", "Failed to Get: Owner:Animator");
	}

	m_mPivot		= m_pModel->Get_Pivot();
	m_pShardPoint	= m_pModel->Get_Bone("hand.l")->Get_CombinedTransformationPointer();

	return S_OK;
}

void CBrotherNodeLeaf_Shard::Activate()
{
	__super::Activate();

	m_fShardTime = 0.f;

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::LUNARSHARD_FIRE_FORWARD, 2.f, true, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_Shard::Invoke(_float _fTimeDelta)
{
	m_fShardTime += _fTimeDelta;

	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::LUNARSHARD_FIRE_FORWARD))
	{
		if (m_pAnimator->Is_Finished(ANIMATION::BROTHER::LUNARSHARD_FIRE_FORWARD))
		{
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);
			m_pAnimator->Fix_Animation(ANIMATION::BROTHER::LUNARSHARD_FIRE_FORWARD, 0.f);
		}
	}

	if (0.2f < m_fShardTime)
	{
		m_pPool->Pop(make_pair(m_pPool, _float3((XMLoadFloat4x4(m_pShardPoint) * m_mPivot * m_pTransform->Get_Matrix()).r[3])));
		m_fShardTime -= 0.2f;
	}

	if (m_fTimeAcc >= 2.f)
	{
		m_eStatus = STATUS::SUCCESS;
	}

	return Return_Invoke();
}

void CBrotherNodeLeaf_Shard::Terminate()
{
	__super::Terminate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::LUNARSHARD_FIRE_FORWARD, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);
}

shared_ptr<CBrotherNodeLeaf_Shard> CBrotherNodeLeaf_Shard::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_Shard> pInstance = make_private_shared(CBrotherNodeLeaf_Shard);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Shard::Create", "Failed to Initialize");
	}

	return pInstance;
}
