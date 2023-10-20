#pragma once
#include "Client_Define.h"
#include "BehaviorTree.h"

#include "LunarExploderNode_Root.h"
#include "LunarExploderNodeLeaf_Idle.h"
#include "LunarExploderNodeLeaf_Spawn.h"
#include "LunarExploderNodeLeaf_Death.h"
#include "LunarExploderNodeLeaf_Sprint.h"
#include "LunarExploderNodeDecorator_Repeat.h"
#include "LunarExploderNodeDecorator_Delay.h"
#include "LunarExploderNodeDecorator_Range.h"
#include "LunarExploderNodeDecorator_Skill.h"
#include "LunarExploderNodeSelector_Main.h"

BEGIN(Client)

class CLunarExploder_BehaviorTree final : public CBehaviorTree
{
private:
	explicit CLunarExploder_BehaviorTree() DEFAULT;
	virtual ~CLunarExploder_BehaviorTree() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*) override;

public:
	static shared_ptr<CLunarExploder_BehaviorTree>	Create(shared_ptr<CGameObject> pOwner, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>					Clone(any = g_aNull) override;
};

END
