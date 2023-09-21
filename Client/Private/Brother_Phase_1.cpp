#include "ClientPCH.h"
#include "Brother_Phase_1.h"
#include "GameInstance.h"

HRESULT CBrother_Phase_1::Initialize(shared_ptr<CBrother_Phase> _pPrevious)
{
	if (FAILED(__super::Initialize(_pPrevious)))
	{
		MSG_RETURN(E_FAIL, "CBrother_Phase_1::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CBrother_Phase_1::Tick(_float _fTimeDelta)
{
}

void CBrother_Phase_1::Late_Tick(_float _fTimeDelta)
{
}

void CBrother_Phase_1::Handle_State(_float _fTimeDelta)
{
}

shared_ptr<CBrother_Phase_1> CBrother_Phase_1::Create(shared_ptr<CBrother_Phase> _pPrevious)
{
	shared_ptr<CBrother_Phase_1> pInstance = make_private_shared(CBrother_Phase_1);

	if (FAILED(pInstance->Initialize(_pPrevious)))
	{
		MSG_RETURN(nullptr, "CBrother_Phase_1::Create", "Failed to Initialize");
	}

	return pInstance;
}
