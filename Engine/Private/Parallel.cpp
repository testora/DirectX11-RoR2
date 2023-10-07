#include "EnginePCH.h"
#include "Parallel.h"

CParallel::CParallel(POLICY _eSuccess, POLICY _eFailure)
	: m_eSuccessPolicy(_eSuccess)
	, m_eFailurePolicy(_eFailure)
{
}

void CParallel::Activate()
{
	__super::Activate();

	for (auto& pChild : m_lstChildren)
	{
		pChild->Activate();
	}
}

STATUS CParallel::Invoke(_float _fTimeDelta)
{
	if (m_lstChildren.empty())
	{
		return STATUS::FAILURE;
	}

	size_t nSuccessCnt(0), nFailureCnt(0), nTerminatedCnt(0);

	for (auto& pChild : m_lstChildren)
	{
		if (pChild->Is_Terminated())
		{
			++nTerminatedCnt;
			continue;
		}

		switch (pChild->Invoke(_fTimeDelta))
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
		if (nTerminatedCnt == m_lstChildren.size())
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}

	return m_eStatus;
}

void CParallel::Terminate()
{
	__super::Terminate();

	for (auto& pChild : m_lstChildren)
	{
		if (pChild->Is_Running())
		{
			pChild->Terminate();
		}
	}
}
