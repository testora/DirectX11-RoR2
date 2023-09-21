#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "Terrain.h"
#include "Model.h"
#include "GolemPlains.h"
#include "RailGunner.h"
#include "RailGunner_PistolBullet.h"
#include "Golem.h"

HRESULT CLoader::Load_Test()
{
#pragma region Prototype Texture
#pragma endregion
#pragma region Prototype Model

//	PROTOTYPE_COMPONENT_MODEL_RAILGUNNER
//	PROTOTYPE_COMPONENT_MODEL_GOLEM
//	PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_RAILGUNNER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/RailGunner/RailGunner.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_RAILGUNNER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_GOLEM,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/Golem/Golem.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_GOLEM");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Map/GolemPlains/Terrain.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS");
	}
	
#ifdef _DEBUG
	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_CUBE,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/_Temp/Model/Cube.fbx")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_CUBE");
	}
#endif

#pragma endregion
#pragma region Prototype VIBuffer
#pragma endregion
#pragma region Prototype GameObject

//	PROTOTYPE_GAMEOBJECT_CAMERA_MAIN
//	PROTOTYPE_GAMEOBJECT_GOLEMPLAINS
//	PROTOTYPE_GAMEOBJECT_RAILGUNNER
//	PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET
//	PROTOTYPE_GAMEOBJECT_GOLEM

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_GOLEMPLAINS,
		CGolemPlains::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_GOLEMPLAINS");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_RAILGUNNER,
		CRailGunner::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_RAILGUNNER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET,
		CRailGunner_PistolBullet::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_GOLEM,
		CGolem::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_GOLEM");
	}

#pragma endregion

	return S_OK;
}
