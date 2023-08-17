#include "ClientPCH.h"
#include "Scene_Test.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "ObjectLayer.h"

CScene_Test::CScene_Test(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::TEST)
{
}

HRESULT CScene_Test::Initialize()
{
	CGameInstance::Get_Instance()->Show_Cursor(false);
#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Enable();
#endif

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Terrain()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Terrain");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Player");
	}

	return S_OK;
}

void CScene_Test::Tick(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(MAINCAM_TOGGLE_MOUSE))
	{
		CGameInstance::Get_Instance()->Toggle_Cursor();
	}
}

void CScene_Test::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CScene_Test::Render()
{
	return S_OK;
}

HRESULT CScene_Test::Ready_Camera()
{
	auto pLayer_Camera = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, SCENE_TEST_LAYER_CAMERA);
	if (nullptr == pLayer_Camera)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to Add_Layer: SCENE_TEST_LAYER_CAMERA");
	}

	if (FAILED(pLayer_Camera->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

	return S_OK;
}

HRESULT CScene_Test::Ready_Terrain()
{
	auto pLayer_Terrain = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, SCENE_TEST_LAYER_TERRAIN);
	if (nullptr == pLayer_Terrain)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Add_Layer: SCENE_TEST_LAYER_TERRAIN");
	}

	if (FAILED(pLayer_Terrain->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TERRAIN))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_TERRAIN");
	}

	return S_OK;
}

HRESULT CScene_Test::Ready_Player()
{
	auto pLayer_Player = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, SCENE_TEST_LAYER_PLAYER);
	if (nullptr == pLayer_Player)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to Add_Layer: SCENE_TEST_LAYER_PLAYER");
	}

	if (FAILED(pLayer_Player->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_RAILGUNNER))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_RAILGUNNER");

	}

	return S_OK;
}

shared_ptr<CScene_Test> CScene_Test::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Test> pInstance = make_private_shared(CScene_Test, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Test::Create", "Failed to Initialize");
	}

	return pInstance;
}