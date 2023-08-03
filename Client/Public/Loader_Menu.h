#pragma once
#include "Loader.h"
#include "GameInstance.h"

#include "Background.h"

HRESULT CLoader::Load_Menu()
{
	if (FAILED(CGameInstance::Get_Instance()->Add_Component_Prototype(PROTOTYPE_COMPONENT_TEXTURE_BACKGROUND,
		CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/Background.png")))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_TEXTURE_BACKGROUND");
	}

	if (FAILED(CGameInstance::Get_Instance()->Add_Object_Prototype(PROTOTYPE_GAMEOBJECT_BACKGROUND,
		CBackground::Create(m_pDevice, m_pContext))))
	{
		MSG_BOX("CLoader::Load_Menu", "Add_Object_Prototype: PROTOTYPE_GAMEOBJECT_BACKGROUND");
	}

	return S_OK;
}
