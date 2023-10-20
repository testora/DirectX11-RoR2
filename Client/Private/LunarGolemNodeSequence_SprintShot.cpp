#include "ClientPCH.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"
#include "LunarGolemNodeSequence_SprintShot.h"

HRESULT CLunarGolemNodeSequence_SprintShot::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolemNodeSequence_SprintShot::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CLunarGolemNodeSequence_SprintShot::Activate()
{
	__super::Activate();
}

STATUS CLunarGolemNodeSequence_SprintShot::Invoke(_float _fTimeDelta)
{
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CLunarGolemNodeSequence_SprintShot::Terminate()
{
	__super::Terminate();
}

shared_ptr<CLunarGolemNodeSequence_SprintShot> CLunarGolemNodeSequence_SprintShot::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CLunarGolemNodeSequence_SprintShot> pInstance = make_private_shared(CLunarGolemNodeSequence_SprintShot);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CLunarGolemNodeSequence_SprintShot::Create", "Failed to Initialize");
	}

	return pInstance;
}
