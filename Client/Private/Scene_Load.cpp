#include "ClientPCH.h"
#include "Scene_Load.h"
#include "Loader.h"
#include "Scene_Load.h"
#include "Scene_Menu.h"
#include "Scene_Tool.h"
#include "Scene_Test.h"
#include "GameInstance.h"

CScene_Load::CScene_Load(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eLoadScene)
	: CScene(_pDevice, _pContext, SCENE::LOADING)
	, m_eLoadScene(_eLoadScene)
{
}

HRESULT CScene_Load::Initialize()
{
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eLoadScene);

	if (nullptr == m_pLoader)
	{
		MSG_RETURN(E_FAIL, "CScene_Load::Initialize", "Failed To CLoader::Create");
	}

	return S_OK;
}

void CScene_Load::Tick(_float _fTimeDelta)
{
}

void CScene_Load::Late_Tick(_float _fTimeDelta)
{
	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText().c_str());

	if (m_pLoader->Is_Finished())
	{
		shared_ptr<CScene> pScene;

		switch (m_eLoadScene)
		{
		case SCENE::MENU:
			pScene = CScene_Menu::Create(m_pDevice, m_pContext);
			break;

#if ACTIVATE_TOOL
		case SCENE::TOOL:
			pScene = CScene_Tool::Create(m_pDevice, m_pContext);
			break;
#endif

		case SCENE::TEST:
			pScene = CScene_Test::Create(m_pDevice, m_pContext);
			break;
		}

		if (nullptr == pScene)
		{
			MSG_RETURN(, "CScene_Load::Late_Tick", "Failed To CScene::Create");
		}

		if (FAILED(CGameInstance::Get_Instance()->Open_Scene(m_eLoadScene, pScene)))
		{
			MSG_RETURN(, "CScene_Load::Late_Tick", "Failed To Open_Scene");
		}
	}
}

HRESULT CScene_Load::Render()
{
	return S_OK;
}

shared_ptr<CScene_Load> CScene_Load::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE eLoadScene)
{
	shared_ptr<CScene_Load> pInstance = make_private_shared(CScene_Load, _pDevice, _pContext, eLoadScene);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CScene_Load", "Failed To CInitialize");
	}

	return pInstance;
}
