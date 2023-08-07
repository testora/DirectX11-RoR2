#include "EnginePCH.h"
#include "Component_Manager.h"

HRESULT CComponent_Manager::Reserve_Manager(const SCENE _eScene)
{
	m_arrComponentPrototypes = Function::CreateDynamicArray<ComponentPrototype>(IDX(_eScene), false);

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag, shared_ptr<CComponent> _pPrototype)
{
	if (nullptr != Find_Prototype(_eScene, _strPrototypeTag))
	{
		MSG_RETURN(E_FAIL, "CComponent_Manager::Add_Prototype", "Already Exists: CComponent");
	}

	m_arrComponentPrototypes[IDX(_eScene)].emplace(_strPrototypeTag, _pPrototype);

	return S_OK;
}

shared_ptr<CComponent> CComponent_Manager::Clone_Component(const SCENE _eScene, const wstring& _strPrototypeTag, any _arg)
{
	shared_ptr<CComponent> pComponent = Find_Prototype(_eScene, _strPrototypeTag);
	if (nullptr == pComponent)
	{
		MSG_RETURN(nullptr, "CComponent_Manager::Clone_Component", "Failed to Find_Prototype");
	}

	return pComponent->Clone(_arg);
}

shared_ptr<CComponent> CComponent_Manager::Find_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag)
{
	auto& ComponentPrototype = m_arrComponentPrototypes[IDX(_eScene)];
	auto iter = ComponentPrototype.find(_strPrototypeTag);
	if (iter == ComponentPrototype.end())
	{
		return nullptr;
	}

	return iter->second;
}
