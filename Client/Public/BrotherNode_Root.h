#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Client)

class CBrotherNode_Root final : public CNode
{
private:
	explicit CBrotherNode_Root() DEFAULT;
	virtual ~CBrotherNode_Root() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void							Activate() override;
	virtual STATUS							Invoke(_float fTimeDelta) override;
	virtual void							Terminate() override;

private:
	HRESULT									Ready_Skills();

private:
	const BROTHER_PHASE*					m_pPhase			= nullptr;
	shared_ptr<CNode>						m_pPhaseSelector;

	map<wstring, SKILLDESC>					m_mapSkills;

public:
	static shared_ptr<CBrotherNode_Root>	Create(shared_ptr<class CBlackBoard>);
};

END

/*
NOTE: 
ROOT
CHILD	[DECORATOR-REPEAT]
		[SELECTOR] PHASE
		CHILD	[SELECTOR] PHASE1
				CHILD	[DECORATOR-SKILL]
						[SEQUENCE] SPECIAL
						CHILD	[LEAF] LEAP_BEGIN
						CHILD	[LEAF] LEAP_END
				//		CHILD	[LEAF] SHOCK_WAVE

				CHILD	[SELECTOR] PRIMARY
						CHILD	[DECORATOR-SKILL]
								[DECORATOR-RANGE-OUT-70]
								[PARALLEL] PRIMARY_SHARD
								CHILD	[LEAF] SPRINT
								CHILD	[LEAF] SHARD
						CHILD	[DECORATOR-SKILL]
								[DECORATOR-RANGE-IN-35]
								[SELECTOR] PRIMARY_MELLE
								CHILD	[DECORATOR-SKILL]
										[SEQUENCE] DASHSMASH
										CHILD	[LEAF] DASH 1-0-1-1
										CHILD	[LEAF] SMASH
								CHILD	[LEAF] SMASH

				CHILD	[DECORATOR-SKILL]
						[DECORATOR-RANGE-IN-40]
						[SELECTOR] SECONDARY
						CHILD	[DECORATOR-SKILL]
								[SEQUENCE] DASH_SPRINTSMASH
								CHILD	[LEAF] DASH_SIDE
										[LEAF] SPRINTSMASH
						CHILD	[LEAF] SPRINTSMASH

				CHILD	[DECORATOR-SKILL]
						[DECORATOR-RANGE-40-50]
						[LEAF] DASH	1-0-1-1

				CHILD	[DECORATOR-RANGE-OUT-50-20]
						[DECORATOR-REPEAT-1]
						[LEAF] SPRINT

				CHILD	[DECORATOR-RANGE-OUT-20]
						[DECORATOR-REPEAT-1]
						[LEAF] RUN

				CHILD	[LEAF] DASH 0-1-1-1


		CHILD	[SELECTOR] PHASE2

		CHILD	[SELECTOR] PHASE3
+				CHILD	[SELECTOR] SPECIAL_LUNAR
+						CHILD	[DECORATOR-SKILL]
+								[SEQUENCE] SPECIAL
+								CHILD	[LEAF] LEAP_BEGIN
+								CHILD	[LEAF] LEAP_END
+						//		CHILD	[LEAF] SHOCK_WAVE
+						CHILD	[DECORATOR-SKILL]
+								[SEQUENCE] ULT
+								CHILD	[LEAF] ULT
+								CHILD	[LEAF] ULT
+								CHILD	[LEAF] ULT
+								CHILD	[LEAF] ULT
+								CHILD	[LEAF] ULT

				CHILD	[SELECTOR] PRIMARY
						CHILD	[DECORATOR-SKILL]
								[DECORATOR-RANGE-OUT-70]
								[PARALLEL] PRIMARY_SHARD
								CHILD	[LEAF] SPRINT
								CHILD	[LEAF] SHARD
						CHILD	[DECORATOR-SKILL]
								[DECORATOR-RANGE-IN-35]
+								[SELECTOR] PRIMARY_MELLE_LUNAR
+								CHILD	[DECORATOR-SKILL]
+										[SEQUENCE] DASHSMASH_LUNAR
+										CHILD	[LEAF] DASH 1-0-1-1
+										CHILD	[LEAF] SMASH_LUNAR
+								CHILD	[LEAF] SMASH_LUNAR

				CHILD	[DECORATOR-SKILL]
						[DECORATOR-RANGE-IN-40]
						[SELECTOR] SECONDARY
						CHILD	[DECORATOR-SKILL]
								[SEQUENCE] DASH_SPRINTSMASH
								CHILD	[LEAF] DASH_SIDE
										[LEAF] SPRINTSMASH
						CHILD	[LEAF] SPRINTSMASH

				CHILD	[DECORATOR-SKILL]
						[DECORATOR-RANGE-40-50]
						[LEAF] DASH	1-0-1-1

				CHILD	[DECORATOR-RANGE-OUT-50-20]
						[DECORATOR-REPEAT-1]
						[LEAF] SPRINT

				CHILD	[DECORATOR-RANGE-OUT-20]
						[DECORATOR-REPEAT-1]
						[LEAF] RUN

				CHILD	[LEAF] DASH 0-1-1-1

*/
