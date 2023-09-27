#include "EnginePCH.h"
#include "Composite.h"

HRESULT CComposite::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CComposite::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

HRESULT CComposite::Terminate()
{
	if (FAILED(__super::Terminate()))
	{
		MSG_RETURN(E_FAIL, "CComposite::Terminate", "Failed to __super::Terminate");
	}

	return S_OK;
}

void CComposite::Add_Child(shared_ptr<CNode> _pNode)
{
	if (nullptr == _pNode)
	{
		MSG_RETURN(, "CComposite::Add_Child", "Nullptr Exception: _pNode");
	}

	m_lstChildren.emplace_back(_pNode);
}
