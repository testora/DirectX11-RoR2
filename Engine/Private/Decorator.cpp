#include "EnginePCH.h"
#include "Decorator.h"

CDecorator::CDecorator(shared_ptr<CNode> _pNode)
	: m_pNode(_pNode)
{
}

void CDecorator::Activate()
{
	__super::Activate();
}

void CDecorator::Terminate()
{
	__super::Terminate();
}
