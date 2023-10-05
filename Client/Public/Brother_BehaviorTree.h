#pragma once
#include "Client_Define.h"
#include "BehaviorTree.h"

#include "BrotherNode_Root.h"
#pragma region LEAF
#include "BrotherNodeLeaf_IdleReady.h"
#include "BrotherNodeLeaf_LeapBegin.h"
#include "BrotherNodeLeaf_LeapEnd.h"
#include "BrotherNodeLeaf_Dash.h"
#include "BrotherNodeLeaf_DashSide.h"
#include "BrotherNodeLeaf_Run.h"
#include "BrotherNodeLeaf_Sprint.h"
#include "BrotherNodeLeaf_LunarSmash.h"
#include "BrotherNodeLeaf_Smash.h"
#include "BrotherNodeLeaf_SprintSmash.h"
#include "BrotherNodeLeaf_Shard.h"
#include "BrotherNodeLeaf_Ult.h"
#pragma endregion
#pragma region DECORATOR
#include "BrotherNodeDecorator_Repeat.h"
#include "BrotherNodeDecorator_Delay.h"
#include "BrotherNodeDecorator_Range.h"
#include "BrotherNodeDecorator_Skill.h"
#pragma endregion
#pragma region COMPOSITE
#pragma region PARALLEL
#include "BrotherNodeParallel_PrimaryShard.h"
#pragma endregion
#pragma region SEQUENCE
#include "BrotherNodeSequence_Special.h"
#include "BrotherNodeSequence_LunarDashSmash.h"
#include "BrotherNodeSequence_DashSmash.h"
#include "BrotherNodeSequence_DashSprintSmash.h"
#include "BrotherNodeSequence_Enter.h"
#include "BrotherNodeSequence_Ult.h"
#pragma endregion
#pragma region SELECTOR
#include "BrotherNodeSelector_Phase.h"
#include "BrotherNodeSelector_Phase1.h"
#include "BrotherNodeSelector_Phase2.h"
#include "BrotherNodeSelector_Phase3.h"
#include "BrotherNodeSelector_Phase4.h"
#include "BrotherNodeSelector_LunarPrimaryMelee.h"
#include "BrotherNodeSelector_PrimaryMelee.h"
#include "BrotherNodeSelector_LunarPrimary.h"
#include "BrotherNodeSelector_Primary.h"
#include "BrotherNodeSelector_Secondary.h"
#include "BrotherNodeSelector_LunarSpecial.h"
#pragma endregion
#pragma endregion


BEGIN(Engine)
class CTransform;
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CBrother_BehaviorTree final : public CBehaviorTree
{
private:
	explicit CBrother_BehaviorTree() DEFAULT;
	virtual ~CBrother_BehaviorTree() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*) override;

public:
	static shared_ptr<CBrother_BehaviorTree>	Create(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>				Clone(any = g_aNull) override;
};

END
