#include "EnginePCH.h"
#include "Composite.h"

void CComposite::Activate()
{
	__super::Activate();
}

void CComposite::Terminate()
{
	__super::Terminate();
}

void CComposite::Add_Child(shared_ptr<CNode> _pNode)
{
	if (nullptr == _pNode)
	{
		MSG_RETURN(, "CComposite::Add_Child", "Nullptr Exception: _pNode");
	}

	m_lstChildren.emplace_back(_pNode);
}
