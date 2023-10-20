#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNodeParallel_WalkShot.h"

CLunarGolemNodeParallel_WalkShot::CLunarGolemNodeParallel_WalkShot()
	: CParallel(POLICY::REQUIRE_ALL, POLICY::REQUIRE_ALL)
{
}

HRESULT CLunarGolemNodeParallel_WalkShot::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeParallel_WalkShot::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CLunarGolemNodeDecorator_Repeat::Create(m_pBlackBoard, 2.5f,
			CLunarGolemNodeLeaf_Walk::Create(m_pBlackBoard, 1.f, 1.f, 1.f)));

	Add_Child(
		CLunarGolemNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:TWINSHOT")).value_or(nullptr),
			CLunarGolemNodeLeaf_TwinShot::Create(m_pBlackBoard)));

	return S_OK;
}

void CLunarGolemNodeParallel_WalkShot::Activate()
{
	__super::Activate();

	++m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:TWINSHOT")).value_or(nullptr)->iStock;
}

STATUS CLunarGolemNodeParallel_WalkShot::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarGolemNodeParallel_WalkShot::Terminate()
{
	__super::Terminate();

	--m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:PRIMARY:TWINSHOT")).value_or(nullptr)->iStock;
}

shared_ptr<CLunarGolemNodeParallel_WalkShot> CLunarGolemNodeParallel_WalkShot::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeParallel_WalkShot> pInstance = make_private_shared(CLunarGolemNodeParallel_WalkShot);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeParallel_WalkShot::Create", "Failed to Initialize");
	}

	return pInstance;
}
