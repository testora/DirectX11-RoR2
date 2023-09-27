#include "EnginePCH.h"
#include "Node.h"

HRESULT CNode::Initialize()
{
	m_eStatus = STATUS::RUNNING;

	return S_OK;
}

HRESULT CNode::Terminate()
{
	if (STATUS::RUNNING == m_eStatus)
	{
		m_eStatus = STATUS::MAX;
	}

	return S_OK;
}

STATUS CNode::Update(_float _fTimeDelta)
{
	if (STATUS::RUNNING != m_eStatus)
	{
		Initialize();
	}

	m_eStatus = Tick(_fTimeDelta);

	if (STATUS::RUNNING != m_eStatus)
	{
		Terminate();
	}

	return m_eStatus;
}
