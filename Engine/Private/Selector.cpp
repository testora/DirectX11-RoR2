#include "EnginePCH.h"
#include "Selector.h"

void CSelector::Activate()
{
	__super::Activate();

	m_itCurrentChild = m_lstChildren.begin();
}

STATUS CSelector::Invoke(_float _fTimeDelta)
{
	if (m_lstChildren.end() == m_itCurrentChild)
	{
		return STATUS::FAILURE;
	}

	switch ((*m_itCurrentChild)->Invoke(_fTimeDelta))
	{
	case STATUS::RUNNING:
	{
		m_eStatus = STATUS::RUNNING;
	}
	break;
	case STATUS::SUCCESS:
	{
		m_eStatus = STATUS::SUCCESS;
	}
	break;
	case STATUS::FAILURE:
	{
		++m_itCurrentChild;
		if (m_lstChildren.end() != m_itCurrentChild)
		{
			m_eStatus = STATUS::RUNNING;
		}
		else
		{
			m_eStatus = STATUS::FAILURE;
		}
	}
	break;
	}

	return m_eStatus;
}

void CSelector::Terminate()
{
	__super::Terminate();
}
