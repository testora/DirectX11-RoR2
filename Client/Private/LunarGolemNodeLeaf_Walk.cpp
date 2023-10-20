#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNodeLeaf_Walk.h"

CLunarGolemNodeLeaf_Walk::CLunarGolemNodeLeaf_Walk(_float _fBackward, _float _fLeft, _float _fRight)
	: m_fBackward	(_fBackward)
	, m_fLeft		(_fLeft)
	, m_fRight		(_fRight)
{
}

HRESULT CLunarGolemNodeLeaf_Walk::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Walk::Initialize", "Failed to __super::Initialize");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Walk::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Walk::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Walk::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Walk::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Walk::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CLunarGolemNodeLeaf_Walk::Activate()
{
	__super::Activate();

	_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_float3 vLookTo;

	m_pTransform->LookAt(m_pTargetTransform);

	do
	{
		m_eDirection = static_cast<DIRECTION>(Function::Random({ 0.f, m_fBackward, m_fLeft, m_fRight }));

		switch (m_eDirection)
		{
		case DIRECTION::BACKWARD:
			vLookTo = -m_pTransform->Get_State(TRANSFORM::LOOK);
			m_eAnimation = ANIMATION::LUNAR_GOLEM::WALK_BACKWARD;
			break;
		case DIRECTION::LEFT:
			vLookTo = -m_pTransform->Get_State(TRANSFORM::RIGHT);
			m_eAnimation = ANIMATION::LUNAR_GOLEM::WALK_LEFT;
			break;
		case DIRECTION::RIGHT:
			vLookTo = m_pTransform->Get_State(TRANSFORM::RIGHT);
			m_eAnimation = ANIMATION::LUNAR_GOLEM::WALK_RIGHT;
			break;
		}

	} while (-0.5f > XMVectorGetX(XMVector3Dot(vLook, vLookTo)));

	m_pAnimator->Play_Animation(m_eAnimation);
}

STATUS CLunarGolemNodeLeaf_Walk::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pTransform->LookAt(m_pTargetTransform);

	switch (m_eDirection)
	{
	case DIRECTION::BACKWARD:
		m_pPhysics->Force(TRANSFORM::LOOK, -m_pEntityDesc->fBackwardSpeed, _fTimeDelta);
		break;
	case DIRECTION::LEFT:
		m_pPhysics->Force(TRANSFORM::RIGHT, -m_pEntityDesc->fLeftSpeed, _fTimeDelta);
		break;
	case DIRECTION::RIGHT:
		m_pPhysics->Force(TRANSFORM::RIGHT, m_pEntityDesc->fRightSpeed, _fTimeDelta);
		break;
	}

	return Return_Invoke();
}

void CLunarGolemNodeLeaf_Walk::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeLeaf_Walk> CLunarGolemNodeLeaf_Walk::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fBackward, _float _fLeft, _float _fRight)
{
	shared_ptr<CLunarGolemNodeLeaf_Walk> pInstance = make_private_shared(CLunarGolemNodeLeaf_Walk, _fBackward, _fLeft, _fRight);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeLeaf_Walk::Create", "Failed to Initialize");
	}

	return pInstance;
}
