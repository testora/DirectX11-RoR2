#include "EnginePCH.h"
#include "Component_Manager.h"

HRESULT CComponent_Manager::Add_Prototype(const wstring& _strPrototypeTag, shared_ptr<CComponent> _pPrototype)
{
	if (nullptr != Find_Prototype(_strPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CComponent_Manager::Add_Prototype", "Already Exists: CComponent");
	}

	m_umapPrototype.emplace(_strPrototypeTag, _pPrototype);

	return S_OK;
}

shared_ptr<CComponent> CComponent_Manager::Clone_Component(const wstring& _strPrototypeTag, any _arg)
{
	shared_ptr<CComponent> pComponent = Find_Prototype(_strPrototypeTag);
	if (nullptr == pComponent)
	{
		MSG_RETURN(nullptr, "CComponent_Manager::Clone_Component", "Failed to Find_Prototype");
	}

	return pComponent->Clone(_arg);
}

shared_ptr<CComponent> CComponent_Manager::Find_Prototype(const wstring& _strPrototypeTag)
{
	auto iter = m_umapPrototype.find(_strPrototypeTag);
	if (iter == m_umapPrototype.end())
	{
		return nullptr;
	}

	return m_umapPrototype[_strPrototypeTag];
}
