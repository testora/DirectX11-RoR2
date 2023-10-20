#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolemNodeDecorator_Skill.h"

CLunarGolemNodeDecorator_Skill::CLunarGolemNodeDecorator_Skill(shared_ptr<CNode> _pNode, SKILLDESC* _pSkillDesc)
	: CDecorator(_pNode)
	, m_pSkillDesc(_pSkillDesc)
{
}

HRESULT CLunarGolemNodeDecorator_Skill::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeDecorator_Skill::Initialize", "Failed to __super::Initialize");
	}

	if (nullptr == m_pSkillDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeDecorator_Skill::Initialize", "Invalid SkillDesc");
	}

	return S_OK;
}

void CLunarGolemNodeDecorator_Skill::Activate()
{
	__super::Activate();

	if (m_pSkillDesc->iStock)
	{
		m_bAvailable = true;
		--m_pSkillDesc->iStock;
	}
	else
	{
		m_bAvailable = false;
	}
}

STATUS CLunarGolemNodeDecorator_Skill::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_bAvailable)
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CLunarGolemNodeDecorator_Skill::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeDecorator_Skill> CLunarGolemNodeDecorator_Skill::Create(shared_ptr<CBlackBoard> _pBlackBoard, SKILLDESC* _pSkillDesc, shared_ptr<CNode> _pNode)
{
	shared_ptr<CLunarGolemNodeDecorator_Skill> pInstance = make_private_shared(CLunarGolemNodeDecorator_Skill, _pNode, _pSkillDesc);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeDecorator_Skill::Create", "Failed to Initialize");
	}

	return pInstance;
}
