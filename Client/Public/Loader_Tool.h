#pragma once
#include "Loader.h"
#include "GameInstance.h"
#include "VFX_ParticleMesh.h"
#include "VIBufferInstance_Mesh.h"

HRESULT CLoader::Load_Tool()
{
#pragma region Prototype Texture
#pragma endregion
#pragma region Prototype Model
#pragma endregion
#pragma region Prototype VIBuffer
#pragma endregion
#pragma region Prototype GameObject

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(SCENE::TEST, PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH,
		CVFX_ParticleMesh::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CLoader::Load_Tool", "Failed to Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH");
	}

#pragma endregion

	return S_OK;
}
