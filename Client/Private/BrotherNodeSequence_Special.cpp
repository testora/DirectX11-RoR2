#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSequence_Special.h"

HRESULT CBrotherNodeSequence_Special::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_Special::Initialize", "Failed to __super::Initialize");
	}

	m_pAnimator = m_pBlackBoard->Get_System<CAnimator>(TEXT("Owner:Animator"));
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_Special::Initialize", "Failed to Get: Owner:Animator");
	}

	m_pSkillDesc = m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SPECIAL:ULT")).value_or(nullptr);
	if (nullptr == m_pSkillDesc)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_Special::Initialize", "Failed to Get: Owner:Animator");
	}

	Add_Child(
		CBrotherNodeDecorator_Delay::Create(m_pBlackBoard, 3.f,
			CBrotherNodeLeaf_LeapBegin::Create(m_pBlackBoard)));
	Add_Child(CBrotherNodeLeaf_LeapEnd::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSequence_Special::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSequence_Special::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSequence_Special::Terminate()
{
	__super::Terminate();

	m_pAnimator->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);

	m_pSkillDesc->iMaxStock = 1;
}

shared_ptr<CBrotherNodeSequence_Special> CBrotherNodeSequence_Special::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSequence_Special> pInstance = make_private_shared(CBrotherNodeSequence_Special);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSequence_Special::Create", "Failed to Initialize");
	}

	return pInstance;
}
