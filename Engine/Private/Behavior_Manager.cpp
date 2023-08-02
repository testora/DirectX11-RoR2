#include "EnginePCH.h"
#include "Behavior_Manager.h"

HRESULT CBehavior_Manager::Add_Prototype(const wstring& _strPrototypeTag, shared_ptr<CBehavior> _pPrototype)
{
	if (nullptr != Find_Prototype(_strPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CBehavior_Manager::Add_Prototype", "Already Exists: CBehavior");
	}

	m_umapPrototype.emplace(_strPrototypeTag, _pPrototype);

	return S_OK;
}

shared_ptr<CBehavior> CBehavior_Manager::Clone_Behavior(const wstring& _strPrototypeTag, std::any _arg)
{
	shared_ptr<CBehavior> pBehavior = Find_Prototype(_strPrototypeTag);
	if (nullptr == pBehavior)
	{
		MSG_RETURN(nullptr, "CBehavior_Manager::Clone_Behavior", "Failed to Find_Prototype");
	}

	return pBehavior->Clone(_arg);
}

shared_ptr<CBehavior> CBehavior_Manager::Find_Prototype(const wstring& _strPrototypeTag)
{
	auto iter = m_umapPrototype.find(_strPrototypeTag);
	if (iter == m_umapPrototype.end())
	{
		return nullptr;
	}

	return m_umapPrototype[_strPrototypeTag];
}
