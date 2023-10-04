#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeLeaf_Run.h"

HRESULT CBrotherNodeLeaf_Run::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Run::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform		= m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	m_pPhysics			= m_pBlackBoard->Get_System<CPhysics>(TEXT("Owner:Physics"));
	m_pAnimator			= m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));

	m_pTargetTransform	= m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));

	m_pEntityDesc		= m_pBlackBoard->Get_EntityDesc(TEXT("Owner:EntityDesc"));

	return S_OK;
}

void CBrotherNodeLeaf_Run::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeLeaf_Run::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	m_pTransform->LookAt(m_pTargetTransform);
	m_pAnimator->Play_Animation(ANIMATION::BROTHER::RUN_FORWARD_READY);
	m_pPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed, _fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeLeaf_Run::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeLeaf_Run> CBrotherNodeLeaf_Run::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeLeaf_Run> pInstance = make_private_shared(CBrotherNodeLeaf_Run);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeLeaf_Run::Create", "Failed to Initialize");
	}

	return pInstance;
}
