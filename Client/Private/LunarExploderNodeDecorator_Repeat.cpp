#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"
#include "LunarExploderNodeDecorator_Repeat.h"

CLunarExploderNodeDecorator_Repeat::CLunarExploderNodeDecorator_Repeat(shared_ptr<CNode> _pNode, _float _fDuration)
	: CDecorator(_pNode)
	, m_fDuration(_fDuration)
{
}

CLunarExploderNodeDecorator_Repeat::CLunarExploderNodeDecorator_Repeat(shared_ptr<CNode> _pNode, function<_bool()> _fpEscape)
	: CDecorator(_pNode)
	, m_fpEscape(_fpEscape)
{
}

HRESULT CLunarExploderNodeDecorator_Repeat::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploderNodeDecorator_Repeat::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CLunarExploderNodeDecorator_Repeat::Activate()
{
	__super::Activate();
}

STATUS CLunarExploderNodeDecorator_Repeat::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	if (m_fpEscape)
	{
		STATUS eStatus = m_pNode->Invoke(_fTimeDelta);
		if (m_fpEscape())
		{
			return STATUS::RUNNING;
		}
		else
		{
			return eStatus;
		}
	}
	else
	{
		m_eStatus = m_fTimeAcc >= m_fDuration ? STATUS::SUCCESS : m_pNode->Invoke(_fTimeDelta);
	}

	if (STATUS::FAILURE == m_eStatus)
	{
		m_eStatus = STATUS::FAILURE;
	}

	return Return_Invoke();
}

void CLunarExploderNodeDecorator_Repeat::Terminate()
{
	m_pNode->Terminate();

	__super::Terminate();
}

shared_ptr<CLunarExploderNodeDecorator_Repeat> CLunarExploderNodeDecorator_Repeat::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fDuration, shared_ptr<CNode> _pNode)
{
	shared_ptr<CLunarExploderNodeDecorator_Repeat> pInstance = make_private_shared(CLunarExploderNodeDecorator_Repeat, _pNode, _fDuration);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeDecorator_Repeat::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CLunarExploderNodeDecorator_Repeat> CLunarExploderNodeDecorator_Repeat::Create(shared_ptr<CBlackBoard> _pBlackBoard, function<_bool()> _fpEscape, shared_ptr<CNode> _pNode)
{
	shared_ptr<CLunarExploderNodeDecorator_Repeat> pInstance = make_private_shared(CLunarExploderNodeDecorator_Repeat, _pNode, _fpEscape);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarExploderNodeDecorator_Repeat::Create", "Failed to Initialize");
	}

	return pInstance;
}
