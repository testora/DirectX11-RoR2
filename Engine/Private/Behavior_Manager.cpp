#include "EnginePCH.h"
#include "Behavior_Manager.h"

HRESULT CBehavior_Manager::Reserve_Manager(const SCENE _eSceneMax)
{
	m_arrBehaviorPrototypes	= Function::CreateDynamicArray<BehaviorPrototype>(IDX(_eSceneMax), false);

	m_eSceneMax				= _eSceneMax;

	return S_OK;
}

HRESULT CBehavior_Manager::Add_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag, shared_ptr<CBehavior> _pPrototype)
{
	if (nullptr != Find_Prototype(_eScene, _wstrPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CBehavior_Manager::Add_Prototype", "Already Exists: CBehavior");
	}

	m_arrBehaviorPrototypes[IDX(_eScene)].emplace(_wstrPrototypeTag, _pPrototype);

	return S_OK;
}

shared_ptr<CBehavior> CBehavior_Manager::Clone_Behavior(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	shared_ptr<CBehavior> pBehavior = Find_Prototype(_eScene, _wstrPrototypeTag);
	if (nullptr == pBehavior)
	{
		MSG_RETURN(nullptr, "CBehavior_Manager::Clone_Behavior", "Failed to Find_Prototype");
	}

	return pBehavior->Clone(_arg);
}

shared_ptr<CBehavior> CBehavior_Manager::Find_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(nullptr, "CBehavior_Manager::Find_Prototype", "Invalid Range: SCENE");
	}

	auto& BehaviorPrototype = m_arrBehaviorPrototypes[IDX(_eScene)];
	auto iter = BehaviorPrototype.find(_wstrPrototypeTag);
	if (iter == BehaviorPrototype.end())
	{
		return nullptr;
	}

	return iter->second;
}
