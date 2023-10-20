#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNodeLeaf_Sprint.h"

CLunarGolemNodeLeaf_Sprint::CLunarGolemNodeLeaf_Sprint(_float _fForward, _float _fBackward, _float _fLeft, _float _fRight)
	: m_fForward	(_fForward)
	, m_fBackward	(_fBackward)
	, m_fLeft		(_fLeft)
	, m_fRight		(_fRight)
{
}

HRESULT CLunarGolemNodeLeaf_Sprint::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Sprint::Initialize", "Failed to __super::Initialize");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeLeaf_Sprint::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CLunarGolemNodeLeaf_Sprint::Activate()
{
	__super::Activate();

	_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_float3 vLookTo;
	_float	fSpeed(0.f);

	do
	{
		m_eDirection = static_cast<DIRECTION>(Function::Random({ m_fForward, m_fBackward, m_fLeft, m_fRight }));

		m_pTransform->LookAt(m_pTargetTransform);

		switch (m_eDirection)
		{
		case DIRECTION::FORWARD:
			vLookTo = m_pTransform->Get_State(TRANSFORM::LOOK);
			m_eAnimation = ANIMATION::LUNAR_GOLEM::SPRINT_FORWARD;
			fSpeed = m_pEntityDesc->fForwardSpeed * m_pEntityDesc->fSpritPower;
			break;
		case DIRECTION::BACKWARD:
			vLookTo = -m_pTransform->Get_State(TRANSFORM::LOOK);
			m_eAnimation = ANIMATION::LUNAR_GOLEM::SPRINT_FORWARD;
			fSpeed = m_pEntityDesc->fBackwardSpeed * m_pEntityDesc->fSpritPower;
			break;
		case DIRECTION::LEFT:
			vLookTo = -m_pTransform->Get_State(TRANSFORM::RIGHT);
			m_eAnimation = ANIMATION::LUNAR_GOLEM::SPRINT_LEFT;
		//	fSpeed = m_pEntityDesc->fLeftSpeed * m_pEntityDesc->fSpritPower;
			fSpeed = -m_pEntityDesc->fLeftSpeed * m_pEntityDesc->fSpritPower;
			break;
		case DIRECTION::RIGHT:
			vLookTo = m_pTransform->Get_State(TRANSFORM::RIGHT);
			m_eAnimation = ANIMATION::LUNAR_GOLEM::SPRINT_RIGHT;
		//	fSpeed = m_pEntityDesc->fRightSpeed * m_pEntityDesc->fSpritPower;
			fSpeed = -m_pEntityDesc->fRightSpeed * m_pEntityDesc->fSpritPower;
			break;
		}

	} while (-0.5f > XMVectorGetX(XMVector3Dot(vLook, vLookTo)));

	m_pAnimator->Play_Animation(m_eAnimation, 2.f, false, g_fDefaultInterpolationDuration, false);

	m_pPhysics->Force(vLookTo, fSpeed);
}

STATUS CLunarGolemNodeLeaf_Sprint::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pTransform->LookAt(m_pTargetTransform);

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

void CLunarGolemNodeLeaf_Sprint::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeLeaf_Sprint> CLunarGolemNodeLeaf_Sprint::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fForward, _float _fBackward, _float _fLeft, _float _fRight)
{
	shared_ptr<CLunarGolemNodeLeaf_Sprint> pInstance = make_private_shared(CLunarGolemNodeLeaf_Sprint, _fForward, _fBackward, _fLeft, _fRight);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeLeaf_Sprint::Create", "Failed to Initialize");
	}

	return pInstance;
}
