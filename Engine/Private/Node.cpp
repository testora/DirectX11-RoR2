#include "EnginePCH.h"
#include "Node.h"

HRESULT CNode::Initialize(shared_ptr<CBlackBoard> pBlackBoard)
{
	m_pBlackBoard = pBlackBoard;

	return S_OK;
}

void CNode::Activate()
{
	m_eStatus = STATUS::RUNNING;
}

void CNode::Terminate()
{
	if (STATUS::RUNNING == m_eStatus)
	{
		m_eStatus = STATUS::MAX;
	}
}

void CNode::Begin_Invoke()
{
	if (STATUS::RUNNING != m_eStatus)
	{
		Activate();
	}
}

STATUS CNode::Return_Invoke()
{
	STATUS eReturn = m_eStatus;

	if (STATUS::RUNNING != m_eStatus)
	{
		Terminate();
	}

	return eReturn;
}
