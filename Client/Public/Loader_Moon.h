#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "Camera_Main.h"
#include "Model.h"
#include "Moon.h"
#include "RailGunner.h"
#include "RailGunner_PistolBullet.h"
#include "Brother.h"
#include "Brother_LunarShard.h"
#include "LunarExploder.h"
#include "LunarGolem.h"
#include "Effect_Demo_Point.h"
#include "Effect_Demo_Rect.h"
#include "VIBufferInstance_Point.h"
#include "VIBufferInstance_Line.h"
#include "VIBufferInstance_Rect.h"
#include "VIBufferInstance_Mesh.h"
#include "VFX_TrailQuad.h"
#include "VFX_TrailLine.h"
#include "VFX_ParticleMesh.h"
#include "SkyCube.h"

HRESULT CLoader::Load_Moon()
{
#pragma region Data

	std::ifstream inFile(TEXT("Bin/Resources/Data/Prototype/VIInstance.dat"), std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Open File");
	}

	_int	iMaxInstance(0);
	size_t	nSize(0), nLength(0);
	_wchar	wszBuffer[MAX_PATH];

	inFile.read(reinterpret_cast<_byte*>(&nSize), sizeof(size_t));
	for (size_t i = 0; i < nSize; ++i)
	{
	//	KEY
		ZeroMemory(wszBuffer, sizeof(_wchar) * MAX_PATH);
		inFile.read(reinterpret_cast<_byte*>(&nLength), sizeof(size_t));
		inFile.read(reinterpret_cast<_byte*>(wszBuffer), sizeof(_wchar) * nLength);
		wstring	wstrKey = wszBuffer;
		string	strKey = Function::ToString(wstrKey);
	//	PATH
		ZeroMemory(wszBuffer, sizeof(_wchar) * MAX_PATH);
		inFile.read(reinterpret_cast<_byte*>(&nLength), sizeof(size_t));
		inFile.read(reinterpret_cast<_byte*>(wszBuffer), sizeof(_wchar) * nLength);
		wstring wstrPath = wszBuffer;
		string	strPath = Function::ToString(wstrPath);
	//	MAX INSTANCE
		inFile.read(reinterpret_cast<_byte*>(&iMaxInstance), sizeof(_int));

		if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, wstrKey,
			CVIBufferInstance_Mesh::Create(m_pDevice, m_pContext, wstrPath, iMaxInstance))))
		{
			MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to CVIBufferInstance_Mesh::Create");
		}
	}

	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Read File");
	}

	inFile.close();

#pragma endregion
#pragma region Prototype Texture

//	PROTOTYPE_COMPONENT_TEXTURE_SKY
//	PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO
//	PROTOTYPE_COMPONENT_TEXTURE_EFFECT_BROTHER_HAMMERTRAIL

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_TEXTURE_SKY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/SkyBox/sky1.dds")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_SKY");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_TEXTURE_EFFECT_BROTHER_HAMMERTRAIL,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Effect/Brother/texBootWaveRingGradient.png")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_EFFECT_BROTHER_HAMMERTRAIL");
	}

#pragma endregion
#pragma region Prototype Model

//	PROTOTYPE_COMPONENT_MODEL_RAILGUNNER
//	PROTOTYPE_COMPONENT_MODEL_BROTHER
//	PROTOTYPE_COMPONENT_MODEL_MOON

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

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_BROTHER_LUNARSHARD,
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Brother/LunarShardMesh.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_BROTHER_LUNARSHARD");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_LUNAREXPLODER,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/LunarExploder/LunarExploder.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_LUNAREXPLODER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_LUNARGOLEM,
		CModel::Create(m_pDevice, m_pContext, MODEL::ANIM, TEXT("Bin/Resources/Model/LunarGolem/LunarGolem.mdl")))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_LUNARGOLEM");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_MODEL_MOON,
	//	CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Map/Moon/Terrain.mdl")))))
		CModel::Create(m_pDevice, m_pContext, MODEL::NONANIM, TEXT("Bin/Resources/Model/Map/Moon/Terrain___.mdl")))))
		{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_MODEL_MOON");
	}

#pragma endregion
#pragma region Prototype VIBuffer

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT,
		CVIBufferInstance_Point::Create(m_pDevice, m_pContext, 100))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE,
		CVIBufferInstance_Line::Create(m_pDevice, m_pContext, 100))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(SCENE::MOON, PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT,
		CVIBufferInstance_Rect::Create(m_pDevice, m_pContext, 100))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT");
	}

#pragma region Mesh Instance

#pragma endregion
#pragma endregion
#pragma region Prototype GameObject

//	PROTOTYPE_GAMEOBJECT_CAMERA_MAIN
//	PROTOTYPE_GAMEOBJECT_MOON
//	PROTOTYPE_GAMEOBJECT_RAILGUNNER
//	PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET
//	PROTOTYPE_GAMEOBJECT_BROTHER

//	PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE
//	PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD
//	PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_CAMERA_MAIN,
		CCamera_Main::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_CAMERA_MAIN");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_SKYBOX_SKY1,
		CSkyCube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_SKYBOX_SKY1");
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

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_BROTHER_LUNARSHARD,
		CBrother_LunarShard::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_BROTHER_LUNARSHARD");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_LUNAREXPLODER,
		CLunarExploder::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_LUNAREXPLODER");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_LUNARGOLEM,
		CLunarGolem::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_LUNARGOLEM");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE,
		CVFX_TrailLine::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD,
		CVFX_TrailQuad::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::MOON, PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH,
	CVFX_ParticleMesh::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Moon", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH");
	}

#pragma endregion

	return S_OK;
}
