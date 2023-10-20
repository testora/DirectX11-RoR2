#include "ClientPCH.h"
#include "GameInstance.h"
#include "BrotherNodeDecorator_Range.h"

CBrotherNodeDecorator_Range::CBrotherNodeDecorator_Range(shared_ptr<CNode> _pNode, _float _fDistance, _bool _bInRange)
	: CDecorator	(_pNode)
	, m_bInRange	(_bInRange)
	, m_fDistanceMin(_bInRange?0.f:_fDistance)
	, m_fDistanceMax(_bInRange?_fDistance:0.f)
{
}

CBrotherNodeDecorator_Range::CBrotherNodeDecorator_Range(shared_ptr<CNode> _pNode, _float _fDistanceMin, _float _fDistanceMax)
	: CDecorator(_pNode)
	, m_fDistanceMin(_fDistanceMin)
	, m_fDistanceMax(_fDistanceMax)
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
	Begin_Invoke(_fTimeDelta);

	if (m_pNode->Is_Running())
	{
		m_eStatus = m_pNode->Invoke(_fTimeDelta);
	}
	else
	{
		if (m_fDistanceMin && m_fDistanceMax)
		{
			if (Function::InRange(Function::Distance(m_pTransform, m_pTargetTransform), m_fDistanceMin, m_fDistanceMax))
			{
				m_eStatus = m_pNode->Invoke(_fTimeDelta);
			}
			else
			{
				m_eStatus = STATUS::FAILURE;
			}
		}
		else
		{
			if (true == m_bInRange)
			{
				if (Function::Distance(m_pTransform, m_pTargetTransform) <= m_fDistanceMax)
				{
					m_eStatus = m_pNode->Invoke(_fTimeDelta);
				}
				else
				{
					m_eStatus = STATUS::FAILURE;
				}
			}
			else
			{
				if (Function::Distance(m_pTransform, m_pTargetTransform) > m_fDistanceMin)
				{
					m_eStatus = m_pNode->Invoke(_fTimeDelta);
				}
				else
				{
					m_eStatus = STATUS::FAILURE;
				}
			}
		}
	}

	return Return_Invoke();
}

void CBrotherNodeDecorator_Range::Terminate()
{
	m_pNode->Terminate();

	__super::Terminate();
}

shared_ptr<CBrotherNodeDecorator_Range> CBrotherNodeDecorator_Range::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fDistanceMin, _float _fDistanceMax, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBrotherNodeDecorator_Range> pInstance = make_private_shared(CBrotherNodeDecorator_Range, _pNode, _fDistanceMin, _fDistanceMax);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Range::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBrotherNodeDecorator_Range> CBrotherNodeDecorator_Range::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fDistance, _bool _bInRange, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBrotherNodeDecorator_Range> pInstance = make_private_shared(CBrotherNodeDecorator_Range, _pNode, _fDistance, _bInRange);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Range::Create", "Failed to Initialize");
	}

	return pInstance;
}
