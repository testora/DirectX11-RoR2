#include "ClientPCH.h"
#include "Scene_Moon.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Camera_Main.h"
#include "VFX_ParticleMesh.h"

#define SCENE_MOON_FOG_COLOR	_float4(0.75f, 0.85f, 0.95f, 1.00f)
#define SCENE_MOON_FOG_START	0.01f
#define SCENE_MOON_FOG_END		1.00f
#define SCENE_MOON_FOG_AMBIENT	0.0f
#define SCENE_MOON_FOG_MAX		0.60f
#define SCENE_MOON_FOG_POWER	0.25f

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

	if (FAILED(Ready_Effect()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Effect");
	}

	if (FAILED(Ready_Collision()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Collision");
	}

	if (FAILED(Ready_Camera()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Camera");
	}

	if (FAILED(Ready_Player()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Player");
	}

	if (FAILED(Ready_Monster()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Monster");
	}

	if (FAILED(Ready_Background()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Background");
	}

	if (FAILED(Ready_Terrain()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Terrain");
	}

	if (FAILED(Ready_Light()))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Initialize", "Failed to Ready_Light");
	}

	return S_OK;
}

void CScene_Moon::Tick(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(MAINCAM_TOGGLE_MOUSE))
	{
		CGameInstance::Get_Instance()->Toggle_Cursor();
	}

	if (CGameInstance::Get_Instance()->Key_Down('B'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_MONSTER_BROTHER)->Pop(ARENA_CENTER);
	}

	if (CGameInstance::Get_Instance()->Key_Down('N'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_MONSTER_LUNAREXPLODER)->Pop(ARENA_CENTER);
	}

	if (CGameInstance::Get_Instance()->Key_Down('M'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_MONSTER_LUNARGOLEM)->Pop(ARENA_CENTER);
	}

	if (CGameInstance::Get_Instance()->Key_Down('K'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_MONSTER_BROTHER_LUNARSHARD)->Pop(
			make_pair(CGameInstance::Get_Instance()->Find_Pool(SCENE::MOON, POOL_MONSTER_BROTHER_LUNARSHARD), ARENA_CENTER));
	}
}

void CScene_Moon::Late_Tick(_float _fTimeDelta)
{
#if ACTIVATE_TOOL
	static _float4	vFogColor	= SCENE_MOON_FOG_COLOR;
	static _float	fFogStart	= SCENE_MOON_FOG_START;
	static _float	fFogEnd		= SCENE_MOON_FOG_END;
	static _float	fFogAmbient	= SCENE_MOON_FOG_AMBIENT;
	static _float	fFogMax		= SCENE_MOON_FOG_MAX;
	static _float	fFogPower	= SCENE_MOON_FOG_POWER;

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

HRESULT CScene_Moon::Render()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene_Moon::Debug()
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

		ImGui::Begin("SHADOW");
		ImGui::Image(CGameInstance::Get_Instance()->Get_Shadow(0).Get(), ImVec2(200, 200));
		ImGui::Image(CGameInstance::Get_Instance()->Get_Shadow(1).Get(), ImVec2(200, 200));
		ImGui::End();
	}
#endif
}
#endif

HRESULT CScene_Moon::Ready_Light()
{
	LIGHTDESC				tLightDesc{};
	tLightDesc.eLightType	= LIGHTTYPE::DIRECTIONAL;
	tLightDesc.vDirection	= _float3(-0.64f, -0.76f, -0.12f);
//	tLightDesc.vDirection	= _float3(-1.f, -2.f, -1.f);
	tLightDesc.vDiffuse		= _color(1.0f, 1.0f, 1.0f, 1.0f);
	tLightDesc.vSpecular	= _color(0.2f, 0.2f, 0.2f, 1.0f);
	tLightDesc.vAmbient		= _color(1.0f, 1.0f, 1.0f, 1.0f);
//	tLightDesc.vAmbient		= _color(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CGameInstance::Get_Instance()->Add_Light(SCENE::MOON, tLightDesc, nullptr)))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Light", "Failed to Add_Light");
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

HRESULT CScene_Moon::Ready_Background()
{
	shared_ptr<CObjectLayer> pLayer = CGameInstance::Get_Instance()->Add_Layer(SCENE::MOON, LAYER_BACKGROUND);

	if (nullptr == pLayer)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Add_Layer: LAYER_BACKGROUND");
	}

	if (FAILED(pLayer->Add(CGameInstance::Get_Instance()->Clone_GameObject(SCENE::MOON, PROTOTYPE_GAMEOBJECT_SKYBOX_SKY1, true))))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Clone_GameObject: PROTOTYPE_GAMEOBJECT_SKYBOX_SKY1");
	}

	shared_ptr<CRenderer> pRenderer = dynamic_pointer_cast<CRenderer>(CGameInstance::Get_Instance()->Clone_Component(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN));
	if (nullptr == pRenderer)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Clone_Component");
	}
	shared_ptr<CShader> pShader = dynamic_pointer_cast<CShader>(pRenderer->Get_DeferredShader());
	if (nullptr == pShader)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Get_DeferredShader");
	}

	_bool bFogEnable(true);
	if (FAILED(pShader->Bind_RawValue(SHADER_FOG_ENABLE, &bFogEnable, sizeof(_bool))))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Bind_RawValue");
	}
	if (FAILED(pShader->Bind_Vector(SHADER_FOG_COLOR, SCENE_MOON_FOG_COLOR)))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Bind_Vector");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_START, SCENE_MOON_FOG_START)))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Bind_Float");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_END, SCENE_MOON_FOG_END)))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Bind_Float");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_MAX, SCENE_MOON_FOG_MAX)))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Bind_Float");
	}
	if (FAILED(pShader->Bind_Float(SHADER_FOG_POWER, SCENE_MOON_FOG_POWER)))
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Terrain", "Failed to Bind_Float");
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
	shared_ptr<CObjectPool> pPool;

	pPool = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON, POOL_MONSTER_BROTHER_LUNARSHARD, PROTOTYPE_GAMEOBJECT_BROTHER_LUNARSHARD, 100);
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Monster", "Failed to Add_Layer: POOL_MONSTER_BROTHER_LUNARSHARD");
	}

	pPool = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON, POOL_MONSTER_BROTHER, PROTOTYPE_GAMEOBJECT_BROTHER, 1);
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Monster", "Failed to Add_Layer: POOL_MONSTER_BROTHER");
	}

	pPool = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON, POOL_MONSTER_LUNAREXPLODER, PROTOTYPE_GAMEOBJECT_LUNAREXPLODER, 1);
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Monster", "Failed to Add_Layer: POOL_MONSTER_LUNAREXPLODER");
	}

	pPool = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON, POOL_MONSTER_LUNARGOLEM, PROTOTYPE_GAMEOBJECT_LUNARGOLEM, 1);
	if (nullptr == pPool)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Monster", "Failed to Add_Layer: POOL_MONSTER_LUNARGOLEM");
	}

	return S_OK;
}

HRESULT CScene_Moon::Ready_Effect()
{
	shared_ptr<CObjectPool> pPoolTrailLineVfx = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON, POOL_EFFECT_TRAIL_LINE, PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE, 5);
	if (nullptr == pPoolTrailLineVfx)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Effect", "Failed to Add_Layer: PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE");
	}

	shared_ptr<CObjectPool> pPoolTrailQuadVfx = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON, POOL_EFFECT_TRAIL_QUAD, PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD, 5);
	if (nullptr == pPoolTrailQuadVfx)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Effect", "Failed to Add_Layer: PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD");
	}

	shared_ptr<CObjectPool> pPoolParticleMeshVfx = CGameInstance::Get_Instance()->Add_Pool(SCENE::MOON,
		POOL_EFFECT_PARTICLE_MESH_PILLAR, PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH, 5, wstring(TEXT("Bin/Resources/Effect/Brother/Mesh/Pillar.vfx")));
	if (nullptr == pPoolParticleMeshVfx)
	{
		MSG_RETURN(E_FAIL, "CScene_Moon::Ready_Effect", "Failed to Add_Layer: PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH");
	}

	return S_OK;
}

HRESULT CScene_Moon::Ready_Collision()
{
	CGameInstance::Get_Instance()->Reset_CollisionGroupCheck();
	CGameInstance::Get_Instance()->Check_CollisionGroup(COLLISION_GROUP::PLAYER, COLLISION_GROUP::MONSTER_BULLET);
	CGameInstance::Get_Instance()->Check_CollisionGroup(COLLISION_GROUP::MONSTER, COLLISION_GROUP::PLAYER_BULLET);
	CGameInstance::Get_Instance()->Check_CollisionGroup(COLLISION_GROUP::PLAYER, COLLISION_GROUP::MONSTER);

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
