#include "ClientPCH.h"
#include "GameInstance.h"
#include "BrotherNodeDecorator_Delay.h"

CBrotherNodeDecorator_Delay::CBrotherNodeDecorator_Delay(shared_ptr<CNode> _pNode, _float _fDelay)
	: CDecorator(_pNode)
	, m_fDelay	(_fDelay)
{
}

HRESULT CBrotherNodeDecorator_Delay::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeDecorator_Delay::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBrotherNodeDecorator_Delay::Activate()
{
	__super::Activate();

	m_fTimeAcc = 0.f;
	m_bSuccess = false;
}

STATUS CBrotherNodeDecorator_Delay::Invoke(_float _fTimeDelta)
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

void CBrotherNodeDecorator_Delay::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeDecorator_Delay> CBrotherNodeDecorator_Delay::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fDelay, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBrotherNodeDecorator_Delay> pInstance = make_private_shared(CBrotherNodeDecorator_Delay, _pNode, _fDelay);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Delay::Create", "Failed to Initialize");
	}

	return pInstance;
}
