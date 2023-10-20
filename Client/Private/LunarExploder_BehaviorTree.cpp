#include "ClientPCH.h"
#include "LunarExploder_BehaviorTree.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "LunarExploder.h"

HRESULT CLunarExploder_BehaviorTree::Initialize(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder_BehaviorTree::Initialize", "__super::Initialize Failed");
	}

    m_pBlackBoard->Add_System(TEXT("Owner:Transform"),	_pOwner->Get_Component(COMPONENT::TRANSFORM));
	m_pBlackBoard->Add_System(TEXT("Owner:Physics"),	_pOwner->Get_Behavior(BEHAVIOR::PHYSICS));
	m_pBlackBoard->Add_System(TEXT("Owner:Animator"),	_pOwner->Get_Behavior(BEHAVIOR::ANIMATOR));

	m_pBlackBoard->Add_System(TEXT("Target:Transform"),	Function::Find_Player()->Get_Component(COMPONENT::TRANSFORM));

	m_pBlackBoard->Add_EntityDesc(TEXT("Owner:EntityDesc"), _pEntityDesc);

	m_pBlackBoard->Add_Anything(TEXT("Owner"), dynamic_pointer_cast<CLunarExploder>(_pOwner));

	Set_RootNode(CLunarExploderNode_Root::Create(m_pBlackBoard));

    return S_OK;
}

shared_ptr<CLunarExploder_BehaviorTree> CLunarExploder_BehaviorTree::Create(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	shared_ptr<CLunarExploder_BehaviorTree> pInstance = make_private_shared(CLunarExploder_BehaviorTree);

	if (FAILED(pInstance->Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(nullptr, "CLunarExploder_BehaviorTree::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CLunarExploder_BehaviorTree::Clone(any)
{
	return make_private_shared_copy(CLunarExploder_BehaviorTree, *this);
}
