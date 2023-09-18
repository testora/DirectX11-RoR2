#include "ClientPCH.h"
#include "Scene_Test.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Camera_Main.h"

CScene_Test::CScene_Test(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::TEST)
{
}

HRESULT CScene_Test::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to __super::Initialize");
	}

	CGameInstance::Get_Instance()->Show_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

	if (FAILED(Ready_Light()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Light");
	}

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

	if (FAILED(Ready_Monster()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Monster");
	}

	return S_OK;
}

void CScene_Test::Tick(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(MAINCAM_TOGGLE_MOUSE))
	{
		CGameInstance::Get_Instance()->Toggle_Cursor();
	}

	if (CGameInstance::Get_Instance()->Key_Down('K'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::TEST, POOL_MONSTER_GOLEM)->Pop(g_aNull);
	}
}

void CScene_Test::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CScene_Test::Render()
{
	return S_OK;
}

HRESULT CScene_Test::Ready_Light()
{
	LIGHTDESC				tLightDesc{};
	tLightDesc.eLightType	= LIGHTDESC::LIGHTTYPE::DIRECTIONAL;
	tLightDesc.vDirection	= _float3(1.f, -2.f, 1.f);
//	tLightDesc.vDirection	= _float3(-1.f, -2.f, -1.f);
	tLightDesc.vDiffuse		= _color(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vSpecular	= _color(0.f, 0.f, 0.f, 1.f);
	tLightDesc.vAmbient		= _color(0.3f, 0.3f, 0.3f, 1.f);
//	tLightDesc.vAmbient		= _color(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CGameInstance::Get_Instance()->Add_Lights(SCENE::TEST, tLightDesc, nullptr)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Light", "Failed to Add_Lights");
	}

	return S_OK;
}

HRESULT CScene_Test::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_CAMERA);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Camera", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

	return S_OK;
}

HRESULT CScene_Test::Ready_Terrain()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_TERRAIN);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Add_Layer: LAYER_TERRAIN");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_GOLEMPLAINS))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_GOLEMPLAINS");
	}

	pLayer->Iterate_Objects(
		[&](shared_ptr<CGameObject> _pTerrain)->_bool
		{
			CGameInstance::Get_Instance()->Register_VIBuffer(SCENE::TEST, GRID_TERRAIN, _pTerrain);

			return true;
		}
	);

	return S_OK;
}

HRESULT CScene_Test::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_PLAYER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to Add_Layer: LAYER_PLAYER");
	}

	shared_ptr<CGameObject> pPlayer;
	if (SUCCEEDED(pLayer->Add(pPlayer = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_RAILGUNNER))))
	{
		shared_ptr<CCamera_Main> pMainCam = dynamic_pointer_cast<CCamera_Main>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN));
		if (nullptr == pMainCam)
		{
			MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
		}

		pMainCam->Attach(pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM), PLAYER_CAMERA_OFFSET);
	}
	else
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Player", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_RAILGUNNER");
	}


	return S_OK;
}

HRESULT CScene_Test::Ready_Monster()
{
	shared_ptr<CObjectPool> pPool = CGameInstance::Get_Instance()->Add_Pool(SCENE::TEST, POOL_MONSTER_GOLEM, PROTOTYPE_GAMEOBJECT_GOLEM, 10);
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Monster", "Failed to Add_Layer: POOL_MONSTER_GOLEM");
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