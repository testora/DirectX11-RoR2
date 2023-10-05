#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeLeaf_Dash.h"

CBrotherNodeLeaf_Dash::CBrotherNodeLeaf_Dash(_float _fForward, _float _fBackward, _float _fLeft, _float _fRight)
	: m_fForward	(_fForward)
	, m_fBackward	(_fBackward)
	, m_fLeft		(_fLeft)
	, m_fRight		(_fRight)
{
}

HRESULT CBrotherNodeLeaf_Dash::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Dash::Initialize", "Failed to __super::Initialize");
	}
	
	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Dash::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Dash::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Dash::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Dash::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CBrotherNodeLeaf_Dash::Activate()
{
	__super::Activate();

	m_eDirection = static_cast<DIRECTION>(Function::Random({ m_fForward, m_fBackward, m_fLeft, m_fRight }));
	
	m_pPhysics->Flattern(true, false, true);
	m_pTransform->LookAt(m_pTargetTransform);

	switch (m_eDirection)
	{
	case DIRECTION::FORWARD:
		m_eAnimation = ANIMATION::BROTHER::DASH_FORWARD;
		m_pPhysics->Force(TRANSFORM::LOOK, BROTHER_DASH_FORCE);
		break;
	case DIRECTION::BACKWARD:
		m_eAnimation = ANIMATION::BROTHER::DASH_BACKWARD;
		m_pPhysics->Force(TRANSFORM::LOOK, -BROTHER_DASH_FORCE);
		break;
	case DIRECTION::LEFT:
		m_eAnimation = ANIMATION::BROTHER::DASH_LEFT;
		m_pPhysics->Force(TRANSFORM::RIGHT, -BROTHER_DASH_FORCE);
		break;
	case DIRECTION::RIGHT:
		m_eAnimation = ANIMATION::BROTHER::DASH_RIGHT;
		m_pPhysics->Force(TRANSFORM::RIGHT, BROTHER_DASH_FORCE);
		break;
	}

	m_pAnimator->Play_Animation(m_eAnimation, 2.f, false, g_fDefaultInterpolationDuration, false);
}

STATUS CBrotherNodeLeaf_Dash::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_pAnimator->Is_Playing(m_eAnimation))
	{
		if (m_pAnimator->Is_Finished(m_eAnimation))
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

void CBrotherNodeLeaf_Dash::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_Dash> CBrotherNodeLeaf_Dash::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fForward, _float _fBackward, _float _fLeft, _float _fRight)
{
	shared_ptr<CBrotherNodeLeaf_Dash> pInstance = make_private_shared(CBrotherNodeLeaf_Dash, _fForward, _fBackward, _fLeft, _fRight);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Dash::Create", "Failed to Initialize");
	}

	return pInstance;
}
