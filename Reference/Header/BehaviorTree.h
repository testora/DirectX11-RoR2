#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CBehaviorTree abstract : public CBehavior
{
private:
	explicit CBehaviorTree();
	explicit CBehaviorTree(const CBehaviorTree&);
	virtual ~CBehaviorTree() DEFAULT;

public:
	HRESULT												Initialize();
	virtual void										Tick(_float fTimeDelta) override;

public:
	HRESULT												Set_RootNode(shared_ptr<class CNode> pNode);
	
	HRESULT												Add_BlackBoard(const wstring& wstrKey, shared_ptr<class ISystem>);
	template<typename T>
	shared_ptr<T>										Get_BlackBoard(const wstring& wstrKey);
	shared_ptr<class ISystem>							Get_BlackBoard(const wstring& wstrKey);

private:
	shared_ptr<class CNode>								m_pRootNode;

	unordered_map<wstring, shared_ptr<class ISystem>>	m_umapBlackBoard;
};

#include "BehaviorTree.inl"

END
