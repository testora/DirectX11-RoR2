#include "EnginePCH.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"
#include "Node.h"
#include "GameObject.h"

CBehaviorTree::CBehaviorTree()
	: CBehavior(BEHAVIOR::CUSTOM)
{
}

CBehaviorTree::CBehaviorTree(const CBehaviorTree& _rhs)
	: CBehavior(_rhs)
{
}

HRESULT CBehaviorTree::Initialize(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTree::Initialize", "Failed to __super::Initialize");
	}

	m_pBlackBoard	= CBlackBoard::Create();
	m_pEntityDesc	= _pEntityDesc;

	return S_OK;
}

void CBehaviorTree::Tick(_float _fTimeDelta)
{
	if (nullptr == m_pRootNode)
	{
		MSG_RETURN(, "CBehaviorTree::Tick", "Nullptr Exception: m_pRootNode");
	}

	m_pRootNode->Invoke(_fTimeDelta);
}

HRESULT CBehaviorTree::Set_RootNode(shared_ptr<CNode> _pNode)
{
	if (nullptr == _pNode)
	{
		MSG_RETURN(E_FAIL, "CBehaviorTree::Set_RootNode", "Nullptr Exception: _pNode");
	}

	m_pRootNode = _pNode;

	return S_OK;
}
