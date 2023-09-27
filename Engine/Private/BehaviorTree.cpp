#include "EnginePCH.h"
#include "BehaviorTree.h"
#include "Node.h"

CBehaviorTree::CBehaviorTree()
	: CBehavior(BEHAVIOR::CUSTOM)
{
}

CBehaviorTree::CBehaviorTree(const CBehaviorTree& _rhs)
	: CBehavior			(_rhs)
	, m_umapBlackBoard	(_rhs.m_umapBlackBoard)
{
}

HRESULT CBehaviorTree::Initialize()
{
	return S_OK;
}

void CBehaviorTree::Tick(_float _fTimeDelta)
{
	if (nullptr == m_pRootNode)
	{
		MSG_RETURN(, "CBehaviorTree::Tick", "Nullptr Exception: m_pRootNode");
	}

	m_pRootNode->Update(_fTimeDelta);
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

HRESULT CBehaviorTree::Add_BlackBoard(const wstring& _wstrKey, shared_ptr<ISystem> _pSystem)
{
	if (m_umapBlackBoard.end() != m_umapBlackBoard.find(_wstrKey))
	{
		MSG_RETURN(E_FAIL, "CBehaviorTree::Add_BlackBoard", "Invalid Key");
	}

	m_umapBlackBoard.emplace(_wstrKey, _pSystem);

	return S_OK;
}

shared_ptr<ISystem> CBehaviorTree::Get_BlackBoard(const wstring& _wstrKey)
{
	if (m_umapBlackBoard.end() != m_umapBlackBoard.find(_wstrKey))
	{
		return m_umapBlackBoard[_wstrKey];
	}

	return nullptr;
}
