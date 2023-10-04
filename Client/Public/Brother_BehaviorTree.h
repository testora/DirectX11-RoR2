#pragma once
#include "Client_Define.h"
#include "BehaviorTree.h"

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

/*
NOTE: 
[DECOTATOR-REPEAT]
ROOT
	[DECORATOR-REPEAT]
	[SELECTOR] PHASE
		?=PHASE0
		[SEQUENCE] LEAP_END, IDLE_READY
		?=PHASE1
		[SELECTOR] PHASE1
			?=FIRST ENTER
			[DECORATOR-RANGE]
			[SEQUENCE] SPRINT, SMASH
			?=IDLE
			[DECORATOR-REPEAT]
			IDLE
*/
