#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeDecorator_Delay.h"

CLunarExploderNodeDecorator_Delay::CLunarExploderNodeDecorator_Delay(shared_ptr<CNode> _pNode, _float _fDelay)
	: CDecorator(_pNode)
	, m_fDelay(_fDelay)
{
}

HRESULT CLunarExploderNodeDecorator_Delay::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeDecorator_Delay::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CLunarExploderNodeDecorator_Delay::Activate()
{
	__super::Activate();

	m_fTimeAcc = 0.f;
	m_bSuccess = false;
}

STATUS CLunarExploderNodeDecorator_Delay::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (false == m_bSuccess)
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);

		if (STATUS::SUCCESS == m_eStatus)
		{
			m_eStatus = STATUS::RUNNING;
			m_bSuccess = true;
		}
	}
	else
	{
		m_fTimeAcc += _fTimeDelta;

		if (m_fDelay < m_fTimeAcc)
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}

	return Return_Invoke();
}

void CLunarExploderNodeDecorator_Delay::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarExploderNodeDecorator_Delay> CLunarExploderNodeDecorator_Delay::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fDelay, shared_ptr<CNode> _pNode)
{
	shared_ptr<CLunarExploderNodeDecorator_Delay> pInstance = make_private_shared(CLunarExploderNodeDecorator_Delay, _pNode, _fDelay);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeDecorator_Delay::Create", "Failed to Initialize");
	}

	return pInstance;
}
