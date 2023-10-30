#include "ClientPCH.h"
#include "Scene_Test.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Camera_Main.h"
#include "Scene_Load.h"

#define SCENE_TEST_FOG_COLOR			_float4(0.48f, 0.64f, 0.72f, 1.00f)
#define SCENE_TEST_FOG_START			0.00f
#define SCENE_TEST_FOG_END				0.25f
#define SCENE_TEST_FOG_AMBIENT			0.00f
#define SCENE_TEST_FOG_MAX				0.60f
#define SCENE_TEST_FOG_POWER			0.25f

#define SCENE_TEST_TELEPORTER_POSITION	_float3(-50.f, -155.f, -50.f)

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

	if (FAILED(Ready_SpawnPoint(TEXT("Bin/Resources/Data/SpawnPoint/Test/Data.dat"))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_SpawnPoint");
	}

	if (FAILED(Ready_Effect()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Effect");
	}

	if (FAILED(Ready_Light()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Light");
	}

	if (FAILED(Ready_Collision()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Collision");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Background()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Background");
	}

	if (FAILED(Ready_Interactable()))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Initialize", "Failed to Ready_Background");
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

	if (CGameInstance::Get_Instance()->Key_Down('G'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::TEST, POOL_MONSTER_GOLEM)->Pop(m_vecSpawnPoint[rand() % m_vecSpawnPoint.size()]);
	}

	if (CGameInstance::Get_Instance()->Key_Down('4'))
	{
		CGameInstance::Get_Instance()->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::MOON));
	}
}

void CScene_Test::Late_Tick(_float _fTimeDelta)
{
#if ACTIVATE_TOOL
	static _float4	vFogColor	= SCENE_TEST_FOG_COLOR;
	static _float	fFogStart	= SCENE_TEST_FOG_START;
	static _float	fFogEnd		= SCENE_TEST_FOG_END;
	static _float	fFogAmbient	= SCENE_TEST_FOG_AMBIENT;
	static _float	fFogMax		= SCENE_TEST_FOG_MAX;
	static _float	fFogPower	= SCENE_TEST_FOG_POWER;

	ImGui::Begin("Fog");
	ImGui::InputFloat4("Color", reinterpret_cast<_float*>(&vFogColor));
	ImGui::InputFloat("Start", &fFogStart);
	ImGui::InputFloat("End", &fFogEnd);
	ImGui::InputFloat("Ambient", &fFogAmbient);
	ImGui::InputFloat("Max", &fFogMax);
	ImGui::InputFloat("Power", &fFogPower);
	if (ImGui::Button("Apply"))
	{
		shared_ptr<CRenderer> pRenderer = dynamic_pointer_cast<CRenderer>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));
		shared_ptr<CShader> pShader = dynamic_pointer_cast<CShader>(pRenderer->Get_DeferredShader());

		pShader->Bind_Vector(SHADER_FOG_COLOR, vFogColor);
		pShader->Bind_Float(SHADER_FOG_START, fFogStart);
		pShader->Bind_Float(SHADER_FOG_END, fFogEnd);
		pShader->Bind_Float(SHADER_FOG_AMBIENT, fFogAmbient);
		pShader->Bind_Float(SHADER_FOG_MAX, fFogMax);
		pShader->Bind_Float(SHADER_FOG_POWER, fFogPower);
	}
	ImGui::End();
#endif
}

HRESULT CScene_Test::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_Test::Debug()
{
#if ACTIVATE_TOOL
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		ImGui::Begin("MATERIAL");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_DIFFUSE).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_AMBIENT).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_SPECULAR).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MATERIAL_EMISSIVE).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("NONBLEND");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_NORMAL).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_DEPTH).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("LIGHT");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_DIFFUSE).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_AMBIENT).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_LIGHT_SPECULAR).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_SHADOWDEPTH).Get(), ImVec2(200, 200));
		ImGui::End();

		ImGui::Begin("MASK");
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_PREPROCESS).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_RenderTarget_ShaderResourceView(RENDERTARGET_MASK).Get(), ImVec2(200, 200));
		ImGui::End();
	}
#endif
}
#endif

HRESULT CScene_Test::Ready_SpawnPoint(const wstring& _wstrPath)
{
	std::ifstream inFile(_wstrPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_SpawnPoint", "Failed to Open File");
	}

	size_t nLength(0);

	inFile.read(reinterpret_cast<_byte*>(&nLength),		sizeof(size_t));
	m_vecSpawnPoint.reserve(nLength);

	for (size_t i = 0; i < nLength; ++i)
	{
		_float3 vPoint;
		inFile.read(reinterpret_cast<_byte*>(&vPoint),	sizeof(_float3));
		m_vecSpawnPoint.emplace_back(vPoint);
	}

	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_SpawnPoint", "Failed to Read File");
	}

	inFile.close();

	return S_OK;
}

HRESULT CScene_Test::Ready_Collision()
{
	CGameInstance::Get_Instance()->Reset_CollisionGroupCheck();
	CGameInstance::Get_Instance()->Check_CollisionGroup(COLLISION_GROUP::PLAYER, COLLISION_GROUP::MONSTER_BULLET);
	CGameInstance::Get_Instance()->Check_CollisionGroup(COLLISION_GROUP::MONSTER, COLLISION_GROUP::PLAYER_BULLET);

	return S_OK;
}

HRESULT CScene_Test::Ready_Light()
{
	LIGHTDESC				tLightDesc{};
	tLightDesc.eLightType	= LIGHTTYPE::DIRECTIONAL;
	tLightDesc.vDirection	= _float3(0.36f, -0.93f, 0.07f);
//	tLightDesc.vDirection	= _float3(-1.f, -2.f, -1.f);
	tLightDesc.vDiffuse		= _color(1.0f, 1.0f, 1.0f, 1.0f);
	tLightDesc.vSpecular	= _color(0.2f, 0.2f, 0.2f, 1.0f);
	tLightDesc.vAmbient		= _color(1.0f, 1.0f, 1.0f, 1.0f);
//	tLightDesc.vAmbient		= _color(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CGameInstance::Get_Instance()->Add_Light(SCENE::TEST, tLightDesc, nullptr)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Light", "Failed to Add_Light");
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

HRESULT CScene_Test::Ready_Background()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_BACKGROUND);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_SKYBOX_SKY0))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_SKYBOX_SKY0");
	}

	pLayer->Iterate_Objects(
		[&](shared_ptr<CGameObject> _pTerrain)->_bool
		{
			_pTerrain->Get_Component<CTransform>(COMPONENT::TRANSFORM)->Rotate(TRANSFORM::UP, -XM_PIDIV2);

			return true;
		}
	);

	shared_ptr<CRenderer> pRenderer = dynamic_pointer_cast<CRenderer>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));
	if (nullptr == pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Clone_Component");
	}
	shared_ptr<CShader> pShader = dynamic_pointer_cast<CShader>(pRenderer->Get_DeferredShader());
	if (nullptr == pShader)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Get_DeferredShader");
	}

	_bool bFogEnable(true);
	if (FAILED(pShader->Bind_RawValue(SHADER_FOG_ENABLE, &bFogEnable, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Bind_RawValue");
	}
	if (FAILED(pShader->Bind_Vector(SHADER_FOG_COLOR, SCENE_TEST_FOG_COLOR)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Bind_Vector");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_START, SCENE_TEST_FOG_START)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Bind_Float");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_END, SCENE_TEST_FOG_END)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Bind_Float");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_MAX, SCENE_TEST_FOG_MAX)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Bind_Float");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_POWER, SCENE_TEST_FOG_POWER)))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Terrain", "Failed to Bind_Float");
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

HRESULT CScene_Test::Ready_Interactable()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::TEST, LAYER_INTERACTABLE);
	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Interactable", "Failed to Add_Layer: LAYER_INTERACTABLE");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TELEPORTER, SCENE_TEST_TELEPORTER_POSITION))))
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Interactable", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_TELEPORTER");
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

HRESULT CScene_Test::Ready_Effect()
{
	shared_ptr<CObjectPool> pPoolTrailLineVfx = CGameInstance::Get_Instance()->Add_Pool(SCENE::TEST, POOL_EFFECT_TRAIL_LINE, PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE, 5);
	if (nullptr == pPoolTrailLineVfx)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Effect", "Failed to Add_Layer: PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE");
	}

	shared_ptr<CObjectPool> pPoolTrailQuadVfx = CGameInstance::Get_Instance()->Add_Pool(SCENE::TEST, POOL_EFFECT_TRAIL_QUAD, PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD, 5);
	if (nullptr == pPoolTrailQuadVfx)
	{
		MSG_RETURN(E_FAIL, "CScene_Test::Ready_Effect", "Failed to Add_Layer: PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD");
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