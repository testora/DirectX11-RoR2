#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "Terrain.h"
#include "Model.h"
#include "GolemPlains.h"
#include "RailGunner.h"

HRESULT CLoader::Load_Test()
{
#pragma region Prototype Texture

//	PROTOTYPE_COMPONENT_TEXTURE_TERRAIN

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_TERRAIN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/_Temp/Texture/Tile0.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TERRAIN");
	}

//	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_BRUSH,
//		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/_Temp/Texture/Brush.png")))))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_HEIGHTMAP");
//	}

#pragma endregion
#pragma region Prototype Model

//	PROTOTYPE_COMPONENT_MODEL_RAILGUNNER
//	PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_RAILGUNNER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/_Temp/Model/RailGunner/untitled.fbx"), XMMatrixRotationY(XMConvertToRadians(180.0f))))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_RAILGUNNER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/_Temp/Map/Set 3_ Golemplains.fbx"), XMMatrixRotationY(XMConvertToRadians(180.0f))))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS");
	}

#pragma endregion
#pragma region Prototype VIBuffer

//	PROTOTYPE_COMPONENT_VIBUFFER_TERRAIN

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_VIBUFFER_TERRAIN,
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/_Temp/Texture/Height.bmp")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_TERRAIN");
	}

#pragma endregion
#pragma region Prototype GameObject

//	PROTOTYPE_GAMEOBJECT_CAMERA_MAIN
//	PROTOTYPE_GAMEOBJECT_TERRAIN
//	PROTOTYPE_GAMEOBJECT_GOLEMPLAINS
//	PROTOTYPE_GAMEOBJECT_RAILGUNNER

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_GameObject_Prototype", "Failed to Add_Object_Prototype: CCamera");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_TERRAIN,
		CTerrain::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_TERRAIN");
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

#pragma endregion

	return S_OK;
}
