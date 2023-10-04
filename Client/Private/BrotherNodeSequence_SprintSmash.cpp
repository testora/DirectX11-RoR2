#include "ClientPCH.h"
#include "BlackBoard.h"
#include "GameInstance.h"
#include "BrotherNodeDecorator_Range.h"
#include "BrotherNodeSequence_SprintSmash.h"
#include "BrotherNodeLeaf_Sprint.h"
#include "BrotherNodeLeaf_SprintSmash.h"

HRESULT CBrotherNodeSequence_SprintSmash::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "CBrotherNodeSequence_SprintSmash::Initialize", "Failed to __super::Initialize");
	}

	Add_Child(CBrotherNodeDecorator_Range::Create(m_pBlackBoard,
		CBrotherNodeLeaf_Sprint::Create(m_pBlackBoard), BROTHER_SPRINT_SMASH_DISTANCE, false));
	Add_Child(CBrotherNodeLeaf_SprintSmash::Create(m_pBlackBoard));

	return S_OK;
}

void CBrotherNodeSequence_SprintSmash::Activate()
{
	__super::Activate();
}

STATUS CBrotherNodeSequence_SprintSmash::Invoke(_float _fTimeDelta)
{
	Begin_Invoke();

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void CBrotherNodeSequence_SprintSmash::Terminate()
{
	__super::Terminate();
}

shared_ptr<CBrotherNodeSequence_SprintSmash> CBrotherNodeSequence_SprintSmash::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<CBrotherNodeSequence_SprintSmash> pInstance = make_private_shared(CBrotherNodeSequence_SprintSmash);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "CBrotherNodeSequence_SprintSmash::Create", "Failed to Initialize");
	}

	return pInstance;
}
