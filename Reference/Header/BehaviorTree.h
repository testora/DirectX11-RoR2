#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CBehaviorTree abstract : public CBehavior
{
protected:
	explicit CBehaviorTree();
	explicit CBehaviorTree(const CBehaviorTree&);
	virtual ~CBehaviorTree() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*);
	virtual void										Tick(_float fTimeDelta) override;

public:
	HRESULT												Set_RootNode(shared_ptr<class CNode> pNode);

protected:
	shared_ptr<class CNode>								m_pRootNode;
	shared_ptr<class CBlackBoard>						m_pBlackBoard;

	const ENTITYDESC*									m_pEntityDesc	= nullptr;
};

END
