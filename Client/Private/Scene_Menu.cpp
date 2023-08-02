#include "ClientPCH.h"
#include "Scene_Menu.h"
#include "GameInstance.h"

CScene_Menu::CScene_Menu(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::MENU)
{
}

HRESULT CScene_Menu::Initialize()
{
	return S_OK;
}

void CScene_Menu::Tick(_float _fTimeDelta)
{
}

void CScene_Menu::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CScene_Menu::Render()
{
	return S_OK;
}

shared_ptr<CScene_Menu> CScene_Menu::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Menu> pInstance = make_private_shared(CScene_Menu, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Menu::Create", "Failed to Initialize");
	}

	return pInstance;
}
