#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeDecorator_Range.h"

CBrotherNodeDecorator_Range::CBrotherNodeDecorator_Range(shared_ptr<CNode> _pNode, _float _fDistance, _bool _bInRange)
	: CDecorator	(_pNode)
	, m_fDistance	(_fDistance)
	, m_bInRange	(_bInRange)
{
}

HRESULT CBrotherNodeDecorator_Range::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeDecorator_Range::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform		= m_pBlackBoard->Get_System<CTransform>(TEXT("Owner:Transform"));
	m_pTargetTransform	= m_pBlackBoard->Get_System<CTransform>(TEXT("Target:Transform"));

	return S_OK;
}

void CBrotherNodeDecorator_Range::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeDecorator_Range::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	if (true == m_bInRange)
	{
		if (Function::Distance(m_pTransform, m_pTargetTransform) <= m_fDistance)
		{
			m_eStatus = m_pNode->Invoke(_fTimeDelta);
		}
		else
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}
	else
	{
		if (Function::Distance(m_pTransform, m_pTargetTransform) > m_fDistance)
		{
			m_eStatus = m_pNode->Invoke(_fTimeDelta);
		}
		else
		{
			m_eStatus = STATUS::SUCCESS;
		}
	}

	return Return_Invoke();
}

void CBrotherNodeDecorator_Range::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeDecorator_Range> CBrotherNodeDecorator_Range::Create(shared_ptr<CBlackBoard> _pBlackBoard, shared_ptr<CNode> _pNode, _float _fDistance, _bool _bInRange)
{
	shared_ptr<CBrotherNodeDecorator_Range> pInstance = make_private_shared(CBrotherNodeDecorator_Range, _pNode, _fDistance, _bInRange);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Range::Create", "Failed to Initialize");
	}

	return pInstance;
}
