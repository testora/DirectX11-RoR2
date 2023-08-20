#pragma once
#include "Engine_Define.h"

#include "Physics.h"
#include "Control.h"
#include "Grounding.h"

BEGIN(Engine)

class CBehavior_Manager final : public CSingleton<CBehavior_Manager>
{
private:
	explicit CBehavior_Manager() DEFAULT;
	virtual ~CBehavior_Manager() DEFAULT;

public:
	HRESULT													Reserve_Manager(const SCENE);

public:
	HRESULT													Add_Prototype(const SCENE, const wstring& strPrototypeTag, shared_ptr<CBehavior> pPrototype);

	shared_ptr<CBehavior>									Clone_Behavior(const SCENE, const wstring& strPrototypeTag, any = any());

private:
	shared_ptr<CBehavior>									Find_Prototype(const SCENE, const wstring& strPrototypeTag);

private:
	typedef unordered_map<wstring, shared_ptr<CBehavior>>	BehaviorPrototype;
	unique_ptr<BehaviorPrototype[]>							m_arrBehaviorPrototypes;

	friend CSingleton<CBehavior_Manager>;
};

END
