#include "EnginePCH.h"
#include "Node.h"

HRESULT CNode::Initialize(shared_ptr<CBlackBoard> pBlackBoard)
{
	m_pBlackBoard = pBlackBoard;

	return S_OK;
}

void CNode::Activate()
{
	m_eStatus	= STATUS::RUNNING;

	m_fTimeAcc	= 0.f;
}

void CNode::Terminate()
{
	m_eStatus = STATUS::MAX;
}

void CNode::Begin_Invoke(_float _fTimeDelta)
{
	if (STATUS::RUNNING != m_eStatus)
	{
		Activate();
	}

	m_fTimeAcc += _fTimeDelta;
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
