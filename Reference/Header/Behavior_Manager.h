#pragma once
#include "Engine_Define.h"

#include "Physics.h"
#include "Grounding.h"
#include "Animator.h"
#include "Control.h"

BEGIN(Engine)

class CBehavior_Manager final : public CSingleton<CBehavior_Manager>
{
private:
	explicit CBehavior_Manager() DEFAULT;
	virtual ~CBehavior_Manager() DEFAULT;

public:
	HRESULT													Reserve_Manager(const SCENE);

public:
	HRESULT													Add_Prototype(const SCENE, const wstring& wstrPrototypeTag, shared_ptr<CBehavior> pPrototype);

	shared_ptr<CBehavior>									Clone_Behavior(const SCENE, const wstring& wstrPrototypeTag, any = any());

private:
	shared_ptr<CBehavior>									Find_Prototype(const SCENE, const wstring& wstrPrototypeTag);

private:
	typedef unordered_map<wstring, shared_ptr<CBehavior>>	BehaviorPrototype;
	unique_ptr<BehaviorPrototype[]>							m_arrBehaviorPrototypes;

	SCENE													m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CBehavior_Manager>;
};

END
