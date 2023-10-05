#include "ClientPCH.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherNodeSelector_LunarSpecial.h"

HRESULT CBrotherNodeSelector_LunarSpecial::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSelector_LunarSpecial::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SPECIAL:SHOCKWAVE")).value_or(nullptr),
			CBrotherNodeSequence_Special::Create(m_pBlackBoard)));

	Add_Child(
		CBrotherNodeDecorator_Skill::Create(m_pBlackBoard, m_pBlackBoard->Get_Anything<SKILLDESC*>(TEXT("Owner:Skill:SPECIAL:ULT")).value_or(nullptr),
			CBrotherNodeSequence_Ult::Create(m_pBlackBoard)));

	return S_OK;
}

void CBrotherNodeSelector_LunarSpecial::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSelector_LunarSpecial::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSelector_LunarSpecial::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSelector_LunarSpecial> CBrotherNodeSelector_LunarSpecial::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSelector_LunarSpecial> pInstance = make_private_shared(CBrotherNodeSelector_LunarSpecial);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSelector_LunarSpecial::Create", "Failed to Initialize");
	}

	return pInstance;
}
