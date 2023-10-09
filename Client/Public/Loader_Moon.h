#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "Model.h"
#include "Moon.h"
#include "RailGunner.h"
#include "RailGunner_PistolBullet.h"
#include "Brother.h"
#include "Effect_Demo_Point.h"
#include "Effect_Demo_Rect.h"
#include "VIBufferInstance_Point.h"
#include "VIBufferInstance_Line.h"
#include "VIBufferInstance_Rect.h"
#include "VFX_TrailQuad.h"
#include "VFX_TrailLine.h"

HRESULT CLoader::Load_Moon()
{
#pragma region Prototype Texture

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO");
	}

#pragma endregion
#pragma region Prototype Model

//	PROTOTYPE_COMPONENT_MODEL_RAILGUNNER
//	PROTOTYPE_COMPONENT_MODEL_BROTHER
//	PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_RAILGUNNER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/RailGunner/RailGunner_mod_aim_backpack_pistol_sniper_super.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_RAILGUNNER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_BROTHER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Brother/Brother_mod_aim_flinch_shard.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BROTHER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_MOON,
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Map/Moon/Terrain.mdl")))))
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Map/Moon/Terrain_.mdl")))))
		{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MOON");
	}

#pragma endregion
#pragma region Prototype VIBuffer

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT,
		CVIBufferInstance_Point::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE,
		CVIBufferInstance_Line::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT,
		CVIBufferInstance_Rect::Create(m_pDevice, m_pContext, 10))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT");
	}

#pragma endregion
#pragma region Prototype GameObject

//	PROTOTYPE_GAMEOBJECT_CAMERA_MAIN
//	PROTOTYPE_GAMEOBJECT_MOON
//	PROTOTYPE_GAMEOBJECT_RAILGUNNER
//	PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET
//	PROTOTYPE_GAMEOBJECT_BROTHER

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_MOON,
		CMoon::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_MOON");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_RAILGUNNER,
		CRailGunner::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_RAILGUNNER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET,
		CRailGunner_PistolBullet::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_BROTHER,
		CBrother::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_BROTHER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_EFFECT_DEMO_POINT,
		CEffect_Demo_Point::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EFFECT_DEMO_POINT");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_EFFECT_DEMO_RECT,
		CEffect_Demo_Rect::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EFFECT_DEMO_RECT");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL,
		CVFX_TrailLine::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL");
	}

#pragma endregion

	return S_OK;
}
