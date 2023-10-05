#include "ClientPCH.h"
#include "GameInstance.h"
#include "BrotherNodeDecorator_Skill.h"

CBrotherNodeDecorator_Skill::CBrotherNodeDecorator_Skill(shared_ptr<CNode> _pNode, SKILLDESC* _pSkillDesc)
	: CDecorator	(_pNode)
	, m_pSkillDesc	(_pSkillDesc)
{
}

HRESULT CBrotherNodeDecorator_Skill::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeDecorator_Skill::Initialize", "Failed to __super::Initialize");
	}

	if (nullptr == m_pSkillDesc)
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeDecorator_Skill::Initialize", "Invalid SkillDesc");
	}

	return S_OK;
}

void CBrotherNodeDecorator_Skill::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeDecorator_Skill::Invoke(_float _fTimeDelta)
{
 	Begin_Invoke(_fTimeDelta);

	if (m_pSkillDesc->iStock)
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CBrotherNodeDecorator_Skill::Terminate()
{
	__super::Terminate();

	if (m_pSkillDesc->iStock) --m_pSkillDesc->iStock;
}

shared_ptr<CBrotherNodeDecorator_Skill> CBrotherNodeDecorator_Skill::Create(shared_ptr<CBlackBoard> _pBlackBoard, SKILLDESC* _pSkillDesc, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBrotherNodeDecorator_Skill> pInstance = make_private_shared(CBrotherNodeDecorator_Skill, _pNode, _pSkillDesc);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Skill::Create", "Failed to Initialize");
	}

	return pInstance;
}
