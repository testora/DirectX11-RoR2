#include "ClientPCH.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_HurtWalk.h"

HRESULT CBrotherNodeLeaf_HurtWalk::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtWalk::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtWalk::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtWalk::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtWalk::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtWalk::Initialize", "Failed to Get: Target:Transform");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_HurtWalk::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	return S_OK;
}

void CBrotherNodeLeaf_HurtWalk::Activate()
{
	__super::Activate();

	_float fRelativeAngle = Function::RelativeAngle(m_pTransform, m_pTargetTransform);

	m_eDirection = static_cast<DIRECTION>(Function::Probability({ 1.f, 0.f, 1.f, 1.f }));

	switch (m_eDirection)
	{
	case DIRECTION::FORWARD:
	{
		m_pTransform->Rotate_Lerp(TRANSFORM::UP, fRelativeAngle, 2.f);
	}
	break;
	case DIRECTION::LEFT:
	{
		m_pTransform->Rotate_Lerp(TRANSFORM::UP, fRelativeAngle - XM_PIDIV4, 2.f);
	}
	break;
	case DIRECTION::RIGHT:
	{
		m_pTransform->Rotate_Lerp(TRANSFORM::UP, fRelativeAngle + XM_PIDIV4, 2.f);
	}
	break;
	}

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::HURT_WALK_FORWARD, BROTHER_HURT_POWER, false, BROTHER_HURT_INTERPOLATE);
}

STATUS CBrotherNodeLeaf_HurtWalk::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed * BROTHER_HURT_FORCE_POWER, _fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeLeaf_HurtWalk::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_HurtWalk> CBrotherNodeLeaf_HurtWalk::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_HurtWalk> pInstance = make_private_shared(CBrotherNodeLeaf_HurtWalk);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_HurtWalk::Create", "Failed to Initialize");
	}

	return pInstance;
}
