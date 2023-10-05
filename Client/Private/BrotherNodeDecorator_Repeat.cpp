#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeDecorator_Repeat.h"

CBrotherNodeDecorator_Repeat::CBrotherNodeDecorator_Repeat(shared_ptr<CNode> _pNode, _float fDuration)
	: CDecorator(_pNode)
	, m_fDuration(fDuration)
{
}

CBrotherNodeDecorator_Repeat::CBrotherNodeDecorator_Repeat(shared_ptr<CNode> _pNode, function<_bool()> _fpEscape)
	: CDecorator(_pNode)
	, m_fpEscape(_fpEscape)
{
}

HRESULT CBrotherNodeDecorator_Repeat::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeDecorator_Range::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBrotherNodeDecorator_Repeat::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeDecorator_Repeat::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);
	
	if (m_fpEscape)
	{
		m_eStatus = m_fpEscape() ? STATUS::SUCCESS : m_pNode->Invoke(_fTimeDelta);
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

void CBrotherNodeDecorator_Repeat::Terminate()
{
	m_pNode->Terminate();

	__super::Terminate();
}

shared_ptr<CBrotherNodeDecorator_Repeat> CBrotherNodeDecorator_Repeat::Create(shared_ptr<CBlackBoard> _pBlackBoard, _float _fDuration, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBrotherNodeDecorator_Repeat> pInstance = make_private_shared(CBrotherNodeDecorator_Repeat, _pNode, _fDuration);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Repeat::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBrotherNodeDecorator_Repeat> CBrotherNodeDecorator_Repeat::Create(shared_ptr<CBlackBoard> _pBlackBoard, function<_bool()> _fpEscape, shared_ptr<CNode> _pNode)
{
	shared_ptr<CBrotherNodeDecorator_Repeat> pInstance = make_private_shared(CBrotherNodeDecorator_Repeat, _pNode, _fpEscape);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeDecorator_Repeat::Create", "Failed to Initialize");
	}

	return pInstance;
}
