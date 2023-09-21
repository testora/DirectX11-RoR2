#include "ClientPCH.h"
#include "Scene_Moon.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Camera_Main.h"

#define	ARENA_POINT		_float3(-1900.f, 1060.f, 710.f)
#define	ARENA_CENTER	_float3(-2050.f, 1000.f, 590.f)

CScene_Moon::CScene_Moon(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CScene(_pDevice, _pContext, SCENE::MOON)
{
}

HRESULT CScene_Moon::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to __super::Initialize");
	}

	CGameInstance::Get_Instance()->Show_Cursor(false);
#if ACTIVATE_IMGUI
	CImGui_Manager::Get_Instance()->Enable();
#endif

	if (FAILED(Ready_Light()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Light");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Terrain()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Terrain");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Player");
	}

	if (FAILED(Ready_Monster()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Monster");
	}

	return S_OK;
}

void CScene_Moon::Tick(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(MAINCAM_TOGGLE_MOUSE))
	{
		CGameInstance::Get_Instance()->Toggle_Cursor();
	}

	if (CGameInstance::Get_Instance()->Key_Down('K'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_MONSTER_BROTHER)->Pop(ARENA_CENTER);
	}
}

void CScene_Moon::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CScene_Moon::Render()
{
	return S_OK;
}

HRESULT CScene_Moon::Ready_Light()
{
	LIGHTDESC				tLightDesc{};
	tLightDesc.eLightType	= LIGHTDESC::LIGHTTYPE::DIRECTIONAL;
	tLightDesc.vDirection	= _float3(-0.64f, -0.76f, -0.12f);
//	tLightDesc.vDirection	= _float3(-1.f, -2.f, -1.f);
	tLightDesc.vDiffuse		= _color(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vSpecular	= _color(0.f, 0.f, 0.f, 1.f);
	tLightDesc.vAmbient		= _color(0.3f, 0.3f, 0.3f, 1.f);
//	tLightDesc.vAmbient		= _color(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CGameInstance::Get_Instance()->Add_Lights(SCENE::MOON, tLightDesc, nullptr)))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Light", "Failed to Add_Lights");
	}

	return S_OK;
}

HRESULT CScene_Moon::Ready_Camera()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MOON, LAYER_CAMERA);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Camera", "Failed to Add_Layer: LAYER_CAMERA");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MOON, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN))))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Camera", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

	return S_OK;
}

HRESULT CScene_Moon::Ready_Terrain()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MOON, LAYER_TERRAIN);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Add_Layer: LAYER_TERRAIN");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MOON, PROTOTYPE_GAMEOBJECT_MOON))))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_MOON");
	}

	pLayer->Iterate_Objects(
		[&](shared_ptr<CGameObject> _pTerrain)->_bool
		{
			CGameInstance::Get_Instance()->Register_VIBuffer(SCENE::MOON, GRID_TERRAIN, _pTerrain);

			return true;
		}
	);

	return S_OK;
}

HRESULT CScene_Moon::Ready_Player()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MOON, LAYER_PLAYER);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Player", "Failed to Add_Layer: LAYER_PLAYER");
	}

	shared_ptr<CGameObject> pPlayer;
	if (SUCCEEDED(pLayer->Add(pPlayer = CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MOON, PROTOTYPE_GAMEOBJECT_RAILGUNNER))))
	{
		pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, ARENA_POINT);

		shared_ptr<CCamera_Main> pMainCam = dynamic_pointer_cast<CCamera_Main>(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MOON, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN));
		if (nullptr == pMainCam)
		{
			MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Player", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
		}

		pMainCam->Attach(pPlayer->Get_Component<CTransform>(COMPONENT::TRANSFORM), PLAYER_CAMERA_OFFSET);
	}
	else
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Player", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_RAILGUNNER");
	}

	return S_OK;
}

HRESULT CScene_Moon::Ready_Monster()
{
	shared_ptr<CObjectPool> pPool = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON, POOL_MONSTER_BROTHER, PROTOTYPE_GAMEOBJECT_BROTHER, 1);
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Monster", "Failed to Add_Layer: POOL_MONSTER_BROTHER");
	}

	return S_OK;
}

shared_ptr<CScene_Moon> CScene_Moon::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CScene_Moon> pInstance = make_private_shared(CScene_Moon, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Moon::Create", "Failed to Initialize");
	}

	return pInstance;
}
