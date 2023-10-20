#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeDecorator_Skill.h"

CLunarExploderNodeDecorator_Skill::CLunarExploderNodeDecorator_Skill(shared_ptr<CNode> _pNode, SKILLDESC* _pSkillDesc)
	: CDecorator(_pNode)
	, m_pSkillDesc(_pSkillDesc)
{
}

HRESULT CLunarExploderNodeDecorator_Skill::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeDecorator_Skill::Initialize", "Failed to __super::Initialize");
	}

	if (nullptr == m_pSkillDesc)
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeDecorator_Skill::Initialize", "Invalid SkillDesc");
	}

	return S_OK;
}

void CLunarExploderNodeDecorator_Skill::Activate()
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

STATUS CLunarExploderNodeDecorator_Skill::Invoke(_float _fTimeDelta)
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

void CLunarExploderNodeDecorator_Skill::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarExploderNodeDecorator_Skill> CLunarExploderNodeDecorator_Skill::Create(shared_ptr<CBlackBoard> _pBlackBoard, SKILLDESC* _pSkillDesc, shared_ptr<CNode> _pNode)
{
	shared_ptr<CLunarExploderNodeDecorator_Skill> pInstance = make_private_shared(CLunarExploderNodeDecorator_Skill, _pNode, _pSkillDesc);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeDecorator_Skill::Create", "Failed to Initialize");
	}

	return pInstance;
}
