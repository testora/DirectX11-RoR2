#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolemNodeDecorator_Delay.h"

CLunarGolemNodeDecorator_Delay::CLunarGolemNodeDecorator_Delay(shared_ptr<CNode> _pNode, _float _fDelay)
	: CDecorator(_pNode)
	, m_fDelay(_fDelay)
{
}

HRESULT CLunarGolemNodeDecorator_Delay::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeDecorator_Delay::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CLunarGolemNodeDecorator_Delay::Activate()
{
	__super::Activate();

	m_fTimeAcc = 0.f;
	m_bSuccess = false;
}

STATUS CLunarGolemNodeDecorator_Delay::Invoke(_float _fTimeDelta)
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

void CLunarGolemNodeDecorator_Delay::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeDecorator_Delay> CLunarGolemNodeDecorator_Delay::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fDelay, shared_ptr<CNode> _pNode)
{
	shared_ptr<CLunarGolemNodeDecorator_Delay> pInstance = make_private_shared(CLunarGolemNodeDecorator_Delay, _pNode, _fDelay);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeDecorator_Delay::Create", "Failed to Initialize");
	}

	return pInstance;
}
