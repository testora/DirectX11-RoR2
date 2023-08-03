#include "ClientPCH.h"
#include "Scene_Menu.h"
#include "GameInstance.h"
#include "ObjectLayer.h"

CScene_Menu::CScene_Menu(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::MENU)
{
}

HRESULT CScene_Menu::Initialize()
{
	if (FAILED(Ready_Background()))
	{
		MSG_RETURN(E_FAIL, "CScene_Menu::Initialize", "Failed to Ready_Background");
	}

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

HRESULT CScene_Menu::Ready_Background()
{
	if (FAILED(CGameInstance::Get_Instance()->Add_Layer(SCENE_MENU_LAYER_BACKGROUND)))
	{
		MSG_RETURN(E_FAIL, "CScene_Menu::Ready_Background", "Failed to Add Layer");
	}

	auto pLayerBackground = CGameInstance::Get_Instance()->Find_Layer(SCENE_MENU_LAYER_BACKGROUND);
	if (nullptr == pLayerBackground)
	{
		MSG_RETURN(E_FAIL, "CScene_Menu::Ready_Background", "Failed to Find_Layer");
	}

	if (FAILED(pLayerBackground->Add(CGameInstance::Get_Instance()->Clone_GameObject(PROTOTYPE_GAMEOBJECT_BACKGROUND))))
	{
		MSG_RETURN(E_FAIL, "CScene_Menu::Ready_Background", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_BACKGROUND");
	}

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
