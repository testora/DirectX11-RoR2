#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "Model.h"
#include "Moon.h"
#include "RailGunner.h"
#include "RailGunner_PistolBullet.h"
#include "Brother.h"

HRESULT CLoader::Load_Moon()
{
#pragma region Prototype Texture
#pragma endregion
#pragma region Prototype Model

//	PROTOTYPE_COMPONENT_MODEL_RAILGUNNER
//	PROTOTYPE_COMPONENT_MODEL_BROTHER
//	PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_RAILGUNNER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/RailGunner/RailGunner.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_RAILGUNNER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_BROTHER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Brother/Brother.mdl")))))
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

#pragma endregion

	return S_OK;
}