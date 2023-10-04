#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_Sprint.h"

HRESULT CBrotherNodeLeaf_Sprint::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeLeaf_Sprint::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform		= m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	m_pPhysics			= m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	m_pAnimator			= m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));

	m_pTargetTransform	= m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));

	m_pEntityDesc		= m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));

	return S_OK;
}

void CBrotherNodeLeaf_Sprint::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeLeaf_Sprint::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	m_pTransform->LookAt(m_pTargetTransform);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);
	m_pPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed * m_pEntityDesc->fSpritPower, _fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeLeaf_Sprint::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_Sprint> CBrotherNodeLeaf_Sprint::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_Sprint> pInstance = make_private_shared(CBrotherNodeLeaf_Sprint);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Sprint::Create", "Failed to Initialize");
	}

	return pInstance;
}
