#include "EnginePCH.h"
#include "Sequence.h"

HRESULT CSequence::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CSequence::Initialize", "Failed to __super::Initialize");
	}

	m_itCurrentChild = m_lstChildren.begin();

	return S_OK;
}

STATUS CSequence::Tick(_float _fTimeDelta)
{
	switch ((*m_itCurrentChild)->Update(_fTimeDelta))
	{
	case STATUS::RUNNING:
	{
		m_eStatus = STATUS::RUNNING;
	}
	break;
	case STATUS::SUCCESS:
	{
		++m_itCurrentChild;
		if (m_lstChildren.end() != m_itCurrentChild)
		{
			m_eStatus = STATUS::RUNNING;
		}
		else
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}
	break;
	case STATUS::FAILURE:
	{
		m_eStatus = STATUS::FAILURE;
	}
	break;
	}

	return m_eStatus;
}

HRESULT CSequence::Terminate()
{
	if (FAILED(__super::Terminate()))
	{
		MSG_RETURN(E_FAIL, "CSequence::Terminate", "Failed to __super::Terminate");
	}

	return S_OK;
}

shared_ptr<CSequence> CSequence::Create()
{
	return make_private_shared(CSequence);
}
