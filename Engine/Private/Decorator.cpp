#include "EnginePCH.h"
#include "Decorator.h"

CDecorator::CDecorator(shared_ptr<CNode> _pNode)
	: m_pNode(_pNode)
{
}

HRESULT CDecorator::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CDecorator::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

HRESULT CDecorator::Terminate()
{
	if (FAILED(__super::Terminate()))
	{
		MSG_RETURN(E_FAIL, "CDecorator::Terminate", "Failed to __super::Terminate");
	}

	return S_OK;
}
