#pragma once
#include "Client_Define.h"
#include "BehaviorTree.h"

#include "LunarGolemNode_Root.h"
#include "LunarGolemNodeLeaf_Idle.h"
#include "LunarGolemNodeLeaf_LeapEnd.h"
#include "LunarGolemNodeLeaf_Death.h"
#include "LunarGolemNodeLeaf_Walk.h"
#include "LunarGolemNodeLeaf_Sprint.h"
#include "LunarGolemNodeLeaf_TwinShot.h"
#include "LunarGolemNodeDecorator_Repeat.h"
#include "LunarGolemNodeDecorator_Delay.h"
#include "LunarGolemNodeDecorator_Range.h"
#include "LunarGolemNodeDecorator_Skill.h"
#include "LunarGolemNodeParallel_WalkShot.h"
#include "LunarGolemNodeSequence_SprintShot.h"
#include "LunarGolemNodeSelector_Main.h"

BEGIN(Client)

class CLunarGolem_BehaviorTree final : public CBehaviorTree
{
private:
	explicit CLunarGolem_BehaviorTree() DEFAULT;
	virtual ~CLunarGolem_BehaviorTree() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*) override;

public:
	static shared_ptr<CLunarGolem_BehaviorTree>		Create(shared_ptr<CGameObject> pOwner, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>					Clone(any = g_aNull) override;
};

END
