#include "ClientPCH.h"
#include "$safeitemname$.h"
#include "BlackBoard.h"
#include "GameInstance.h"

HRESULT C$safeitemname$::Initialize(shared_ptr<CBlackBoard> _pBlackBoard)
{
	if (FAILED(__super::Initialize(_pBlackBoard)))
	{
		MSG_RETURN(E_FAIL, "C$safeitemname$::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void C$safeitemname$::Activate()
{
	__super::Activate();
}

STATUS C$safeitemname$::Invoke(_float _fTimeDelta)
{
	return STATUS::RUNNING;
}

void C$safeitemname$::Terminate()
{
	__super::Terminate();
}

shared_ptr<C$safeitemname$> C$safeitemname$::Create(shared_ptr<CBlackBoard> _pBlackBoard)
{
	shared_ptr<C$safeitemname$> pInstance = make_private_shared(C$safeitemname$);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "C$safeitemname$::Create", "Failed to Initialize");
	}

	return pInstance;
}
