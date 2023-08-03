#include "EnginePCH.h"
#include "Behavior_Manager.h"

HRESULT CBehavior_Manager::Reserve_Manager(const SCENE _eScene)
{
	m_arrBehaviorPrototypes = Function::MakeUniqueDynamicArray<BehaviorPrototype>(IDX(_eScene));

	return S_OK;
}

HRESULT CBehavior_Manager::Add_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag, shared_ptr<CBehavior> _pPrototype)
{
	if (nullptr != Find_Prototype(_eScene, _strPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CBehavior_Manager::Add_Prototype", "Already Exists: CBehavior");
	}

	m_arrBehaviorPrototypes[IDX(_eScene)].emplace(_strPrototypeTag, _pPrototype);

	return S_OK;
}

shared_ptr<CBehavior> CBehavior_Manager::Clone_Behavior(const SCENE _eScene, const wstring& _strPrototypeTag, any _arg)
{
	shared_ptr<CBehavior> pBehavior = Find_Prototype(_eScene, _strPrototypeTag);
	if (nullptr == pBehavior)
	{
		MSG_RETURN(nullptr, "CBehavior_Manager::Clone_Behavior", "Failed to Find_Prototype");
	}

	return pBehavior->Clone(_arg);
}

shared_ptr<CBehavior> CBehavior_Manager::Find_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag)
{
	auto& BehaviorPrototype = m_arrBehaviorPrototypes[IDX(_eScene)];
	auto iter = BehaviorPrototype.find(_strPrototypeTag);
	if (iter == BehaviorPrototype.end())
	{
		return nullptr;
	}

	return iter->second;
}
