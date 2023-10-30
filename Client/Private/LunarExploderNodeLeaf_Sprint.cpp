#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeLeaf_Sprint.h"

CLunarExploderNodeLeaf_Sprint::CLunarExploderNodeLeaf_Sprint(_float _fForward, _float _fLeft, _float _fRight)
	: m_fForward	(_fForward)
	, m_fLeft		(_fLeft)
	, m_fRight		(_fRight)
{
}

HRESULT CLunarExploderNodeLeaf_Sprint::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Sprint::Initialize", "Failed to __super::Initialize");
	}

	m_pEntityDesc = m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));
	if (nullptr == m_pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:EntityDesc");
	}

	m_pTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:Transform");
	}
	m_pPhysics = m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:Physics");
	}
	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Sprint::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pTargetTransform = m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeLeaf_Sprint::Initialize", "Failed to Get: Target:Transform");
	}

	return S_OK;
}

void CLunarExploderNodeLeaf_Sprint::Activate()
{
	__super::Activate();

	_float3 vLook = m_pTransform->Get_State(TRANSFORM::LOOK);
	_float3 vLookTo;

	do
	{
		m_eDirection = static_cast<DIRECTION>(Function::Probability({ m_fForward, 0.f, m_fLeft, m_fRight }));

		m_pTransform->LookAt(m_pTargetTransform);

		switch (m_eDirection)
		{
		case DIRECTION::FORWARD:
			vLookTo = m_pTransform->Get_State(TRANSFORM::LOOK);
			break;
		case DIRECTION::LEFT:
			vLookTo = -m_pTransform->Get_State(TRANSFORM::RIGHT);
			break;
		case DIRECTION::RIGHT:
			vLookTo = m_pTransform->Get_State(TRANSFORM::RIGHT);
			break;
		}

	} while (-0.5f > XMVectorGetX(XMVector3Dot(vLook, vLookTo)));

	m_pTransform->LookTo(vLook);
	m_pTransform->LookTo_Interpolation(vLookTo, true, false, 1.f, 1.f);

	m_pAnimator->Play_Animation(ANIMATION::LUNAR_EXPLODER::SPRINT_FORWARD);
}

STATUS CLunarExploderNodeLeaf_Sprint::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_pPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed, _fTimeDelta);

	return Return_Invoke();
}

void CLunarExploderNodeLeaf_Sprint::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarExploderNodeLeaf_Sprint> CLunarExploderNodeLeaf_Sprint::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fForward, _float _fLeft, _float _fRight)
{
	shared_ptr<CLunarExploderNodeLeaf_Sprint> pInstance = make_private_shared(CLunarExploderNodeLeaf_Sprint, _fForward, _fLeft, _fRight);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeLeaf_Sprint::Create", "Failed to Initialize");
	}

	return pInstance;
}
