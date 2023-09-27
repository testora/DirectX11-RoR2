#include "EnginePCH.h"
#include "Parallel.h"

CParallel::CParallel(POLICY _eSuccess, POLICY _eFailure)
	: m_eSuccessPolicy(_eSuccess)
	, m_eFailurePolicy(_eFailure)
{
}

HRESULT CParallel::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CParallel::Initialize", "Failed to __super::Initialize");
	}

	for (auto& pChild : m_lstChildren)
	{
		if (FAILED(pChild->Initialize()))
		{
			MSG_RETURN(E_FAIL, "CParallel::Initialize", "Failed to Initialize Child");
		}
	}

	return S_OK;
}

STATUS CParallel::Tick(_float _fTimeDelta)
{
	size_t nSuccessCnt(0), nFailureCnt(0);

	for (auto& pChild : m_lstChildren)
	{
		if (pChild->Is_Terminated())
		{
			continue;
		}

		switch (pChild->Update(_fTimeDelta))
		{
		case STATUS::RUNNING:
		{
			m_eStatus = STATUS::RUNNING;
		}
		break;
		case STATUS::SUCCESS:
		{
			++nSuccessCnt;
			if (m_eSuccessPolicy == POLICY::REQUIRE_ONE)
			{
				m_eStatus = STATUS::SUCCESS;
			}
		}
		break;
		case STATUS::FAILURE:
		{
			++nFailureCnt;
			if (m_eFailurePolicy == POLICY::REQUIRE_ONE)
			{
				m_eStatus = STATUS::FAILURE;
			}
		}
		break;
		}
	}

	if (STATUS::RUNNING == m_eStatus)
	{
		if (m_eSuccessPolicy == POLICY::REQUIRE_ALL && nSuccessCnt == m_lstChildren.size())
		{
			m_eStatus = STATUS::SUCCESS;
		}
		if (m_eFailurePolicy == POLICY::REQUIRE_ALL && nFailureCnt == m_lstChildren.size())
		{
			m_eStatus = STATUS::FAILURE;
		}
	}

	return m_eStatus;
}

HRESULT CParallel::Terminate()
{
	if (FAILED(__super::Terminate()))
	{
		MSG_RETURN(E_FAIL, "CParallel::Terminate", "Failed to __super::Terminate");
	}

	for (auto& pChild : m_lstChildren)
	{
		if (pChild->Is_Running())
		{
			if (FAILED(pChild->Terminate()))
			{
				MSG_RETURN(E_FAIL, "CParallel::Terminate", "Failed to Terminate Child");
			}
		}
	}

	return S_OK;
}
