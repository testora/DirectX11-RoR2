#include "ClientPCH.h"
#include "Scene_Stage1.h"

CScene_Stage1::CScene_Stage1(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::STAGE1)
{
}

HRESULT CScene_Stage1::Initialize()
{
	return S_OK;
}

void CScene_Stage1::Tick(_float _fTimeDelta)
{
}

void CScene_Stage1::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CScene_Stage1::Render()
{
	return S_OK;
}

shared_ptr<CScene_Stage1> CScene_Stage1::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Stage1> pInstance = make_private_shared(CScene_Stage1, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Stage1::Create", "Failed to Initialize");
	}

	return pInstance;
}