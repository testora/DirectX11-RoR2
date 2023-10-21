#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolemNode_Root.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolem.h"

HRESULT CLunarGolem_BehaviorTree::Initialize(shared_ptr<class CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem_BehaviorTree::Initialize", "__super::Initialize Failed");
	}

    m_pBlackBoard->Add_System(TEXT("Owner:Transform"),	_pOwner->Get_Component(COMPONENT::TRANSFORM));
	m_pBlackBoard->Add_System(TEXT("Owner:Physics"),	_pOwner->Get_Behavior(BEHAVIOR::PHYSICS));
	m_pBlackBoard->Add_System(TEXT("Owner:Animator"),	_pOwner->Get_Behavior(BEHAVIOR::ANIMATOR));

	m_pBlackBoard->Add_System(TEXT("Target:Transform"),	Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));

	m_pBlackBoard->Add_EntityDesc(TEXT("Owner:EntityDesc"), _pEntityDesc);

	m_pBlackBoard->Add_Anything(TEXT("Owner"), dynamic_pointer_cast<CLunarGolem>(_pOwner));

	Set_RootNode(CLunarGolemNode_Root::Create(m_pBlackBoard));

	return S_OK;
}

shared_ptr<CLunarGolem_BehaviorTree> CLunarGolem_BehaviorTree::Create(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	shared_ptr<CLunarGolem_BehaviorTree> pInstance = make_private_shared(CLunarGolem_BehaviorTree);

	if (FAILED(pInstance->Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(nullptr, "CLunarGolem_BehaviorTree::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CLunarGolem_BehaviorTree::Clone(any)
{
	return make_private_shared_copy(CLunarGolem_BehaviorTree, *this);
}
