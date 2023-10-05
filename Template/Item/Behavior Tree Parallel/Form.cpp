#include "ClientPCH.h"
#include "GameInstance.h"
#include "$safeitemname$.h"

C$safeitemname$::C$safeitemname$(POLICY _eSuccess, POLICY _eFailure)
	: CParallel(_eSuccess, _eFailure)
{
}

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
	Begin_Invoke(_fTimeDelta);

	m_eStatus = __super::Invoke(_fTimeDelta);

	return Return_Invoke();
}

void C$safeitemname$::Terminate()
{
	__super::Terminate();
}

shared_ptr<C$safeitemname$> C$safeitemname$::Create(shared_ptr<CBlackBoard> _pBlackBoard, POLICY _eSuccess, POLICY _eFailure)
{
	shared_ptr<C$safeitemname$> pInstance = make_private_shared(C$safeitemname$, _eSuccess, _eFailure);

	if (FAILED(pInstance->Initialize(_pBlackBoard)))
	{
		MSG_RETURN(nullptr, "C$safeitemname$::Create", "Failed to Initialize");
	}

	return pInstance;
}
