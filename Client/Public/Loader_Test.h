#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "Terrain.h"

HRESULT CLoader::Load_Test()
{
#pragma region Prototype Texture

//	PROTOTYPE_COMPONENT_TEXTURE_TERRAIN
//	PROTOTYPE_COMPONENT_TEXTURE_HEIGHTMAP

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_TERRAIN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/_Temp/Texture/Tile0.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_TERRAIN");
	}

//	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::TEST, PROTOTYPE_COMPONENT_TEXTURE_HEIGHTMAP,
//		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/_Temp/Texture/Height.bmp")))))
//	{
//		MSG_RETURN(E_FAIL, "CLoader::Load_Test", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_HEIGHTMAP");
//	}

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

#pragma endregion

	return S_OK;
}
