#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNode_Root.h"

HRESULT CBrotherNode_Root::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to __super::Initialize");
	}

	if (FAILED(Ready_Skills()))
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to Ready_Skills");
	}

	m_pPhaseSelector = CBrotherNodeSelector_Phase::Create(m_pBlackBoard);
	if (nullptr == m_pPhaseSelector)
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to CBrotherNodeSelector_Phase::Create");
	}

	m_pPhase = m_pBlackBoard->Get_Anything<BROTHER_PHASE*>(TEXT("Owner:Phase")).value_or(nullptr);
	if (nullptr == m_pPhase)
	{
		MSG_RETURN(E_FAIL, "CBrotherNode_Root::Initialize", "Failed to Get: Owner:Phase");
	}

	return S_OK;
}

void CBrotherNode_Root::Activate()
{
	__super::Activate();
}

STATUS CBrotherNode_Root::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (BROTHER_PHASE::PHASE0 != *m_pPhase)
	{
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
	}

	m_eStatus = m_pPhaseSelector->Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNode_Root::Terminate()
{
	__super::Terminate();
}

HRESULT CBrotherNode_Root::Ready_Skills()
{
	SKILLDESC tSkill{};

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 4.f;
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime	= -4.f;
	tSkill.fCoefficient		= 1200.f;
	m_mapSkills.emplace(TEXT("PRIMARY:MELEE"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:PRIMARY:MELEE"), &m_mapSkills[TEXT("PRIMARY:MELEE")]);

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 4.f;
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime	= -4.f;
	tSkill.fCoefficient		= 1200.f;
	m_mapSkills.emplace(TEXT("PRIMARY:MELEE:LUNAR"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:PRIMARY:MELEE:LUNAR"), &m_mapSkills[TEXT("PRIMARY:MELEE:LUNAR")]);

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 6.f;
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime = 0.f;
	tSkill.fCoefficient		= 60.f;
	m_mapSkills.emplace(TEXT("PRIMARY:SHARD"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:PRIMARY:SHARD"), &m_mapSkills[TEXT("PRIMARY:SHARD")]);

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 6.f;
	tSkill.iStock			= 1;
	tSkill.fCurrentCoolTime = 0.f;
	tSkill.fCoefficient		= 60.f;
	m_mapSkills.emplace(TEXT("PRIMARY:SHARD:HURT"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:PRIMARY:SHARD:HURT"), &m_mapSkills[TEXT("PRIMARY:SHARD:HURT")]);

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 5;
	tSkill.iStock			= 1;
	tSkill.fCurrentCoolTime = 0.f;
	tSkill.fCoefficient		= 200.f;
	m_mapSkills.emplace(TEXT("SECONDARY"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:SECONDARY"), &m_mapSkills[TEXT("SECONDARY")]);

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 12;
	tSkill.iStock			= 1;
	tSkill.fCurrentCoolTime = -24.f;
	tSkill.fCoefficient		= 200.f;
	m_mapSkills.emplace(TEXT("SECONDARY:FISTSLAM"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:SECONDARY:FISTSLAM"), &m_mapSkills[TEXT("SECONDARY:FISTSLAM")]);

	tSkill.iMaxStock		= 2;
	tSkill.fCoolTime		= 3.f;
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime = 0.f;
	tSkill.fCoefficient		= 0.f;
	m_mapSkills.emplace(TEXT("UTILITY"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:UTILITY"), &m_mapSkills[TEXT("UTILITY")]);

	tSkill.iMaxStock		= 1;
	tSkill.fCoolTime		= 30.f;
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime = 0.f;
	tSkill.fCoefficient		= 400.f;
	m_mapSkills.emplace(TEXT("SPECIAL:SHOCKWAVE"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:SPECIAL:SHOCKWAVE"), &m_mapSkills[TEXT("SPECIAL:SHOCKWAVE")]);

	tSkill.iMaxStock		= 0;
	tSkill.fCoolTime		= 4.f;
	tSkill.iStock			= 0;
	tSkill.fCurrentCoolTime = 0.f;
	tSkill.fCoefficient		= 400.f;
	m_mapSkills.emplace(TEXT("SPECIAL:ULT"), tSkill);
	m_pBlackBoard->Add_Anything(TEXT("Owner:Skill:SPECIAL:ULT"), &m_mapSkills[TEXT("SPECIAL:ULT")]);

	return S_OK;
}

shared_ptr<CBrotherNode_Root> CBrotherNode_Root::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNode_Root> pInstance = make_private_shared(CBrotherNode_Root);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNode_Root::Create", "Failed to Initialize");
	}

	return pInstance;
}
