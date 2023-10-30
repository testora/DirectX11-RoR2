#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNode_Root.h"

HRESULT CLunarGolemNode_Root::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNode_Root::Initialize", "Failed to __super::Initialize");
	}

	if (FAILED(Ready_Skills()))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNode_Root::Initialize", "Failed to Ready_Skills");
	}

	m_pBehaviorSelector = CLunarGolemNodeSelector_Main::Create(m_pBlackBoard);
	if (nullptr == m_pBehaviorSelector)
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNode_Root::Initialize", "Failed to CBrotherNodeSelector_Phase::Create");
	}

	return S_OK;
}

void CLunarGolemNode_Root::Activate()
{
	__super::Activate();
}

STATUS CLunarGolemNode_Root::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);
	
	for (auto& iter : m_mapSkills)
	{
		if (iter.second.iStock < iter.second.iMaxStock)
		{
			iter.second.fCurrentCoolTime += _fTimeDelta;
			if (iter.second.fCurrentCoolTime >= iter.second.fCoolTime)
			{
				iter.second.fCurrentCoolTime = 0.f;
				++iter.second.iStock;
			}
		}
	}

	m_eStatus = m_pBehaviorSelector->Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarGolemNode_Root::Terminate()
{
	__super::Terminate();
}

HRESULT CLunarGolemNode_Root::Ready_Skills()
{
	SKILLDESC tSkill{};

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 3.f;
	tSkill.iStock			= 1;
	tSkill.fCurrentCoolTime	= 0.f;
	tSkill.fCoefficient		= 100.f;
	m_mapSkills.emplace(TEXT("PRIMARY:TWINSHOT"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:PRIMARY:TWINSHOT"), &m_mapSkills[TEXT("PRIMARY:TWINSHOT")]);

	return S_OK;
}

shared_ptr<CLunarGolemNode_Root> CLunarGolemNode_Root::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNode_Root> pInstance = make_private_shared(CLunarGolemNode_Root);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNode_Root::Create", "Failed to Initialize");
	}

	return pInstance;
}
