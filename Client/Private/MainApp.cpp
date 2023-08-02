#include "ClientPCH.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "Renderer.h"
#include "Scene_Menu.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
}

CMainApp::~CMainApp()
{
	CMainApp::Destroy_Instance();
	CGameInstance::Release_Engine();

#if defined(DEBUG) || defined(_DEBUG)
	if (nullptr != m_pDevice)
	{
		ID3D11Debug* d3dDebug;
		HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
			OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");

			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
			OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		}
		if (d3dDebug != nullptr)
		{
			d3dDebug->Release();
		}
	}
#endif
}

HRESULT CMainApp::Initialize()
{
	if (!m_pGameInstance)
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Null Exception: m_pGameInstance");
	}

	GRAPHICDESC tGraphicDesc{};

	tGraphicDesc.eWinMode	= GRAPHICDESC::WINDOW;
	tGraphicDesc.hWnd		= g_hWnd;
	tGraphicDesc.iWinCX		= g_iWinCX;
	tGraphicDesc.iWinCY		= g_iWinCY;

	if (FAILED(m_pGameInstance->Initialize_Engine(tGraphicDesc, m_pDevice, m_pContext)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: m_pGameInstance->Initialize_Engine");
	}

	if (FAILED(Default_Settings()))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: Default_Settings");
	}

	if (FAILED(Ready_Component_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: Ready_Component_Prototype");
	}

	if (FAILED(m_pGameInstance->Open_Scene(SCENE::MENU, CScene_Menu::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Open_Scene");
	}

	return S_OK;
}

#pragma region Timer Management

void CMainApp::Tick_Timer()
{
	if (nullptr != m_pGameInstance)
	{
		m_pGameInstance->Tick_Timer();
	}
}

_bool CMainApp::Check_Timer(const _float _fFPS)
{
	if (nullptr != m_pGameInstance)
	{
		return m_pGameInstance->Check_Timer(_fFPS);
	}

	return false;
}

_float CMainApp::Get_TimeDelta(const _float _fFPS) const
{
	if (nullptr != m_pGameInstance)
	{
		return m_pGameInstance->Get_TimeDelta(_fFPS);
	}

	return 0.f;
}

#pragma endregion

void CMainApp::Tick(_float _fTimeDelta)
{
	if (m_pGameInstance)
	{
		m_pGameInstance->Tick_Engine(_fTimeDelta);
	}
}

HRESULT CMainApp::Render()
{
	if (!m_pGameInstance || !m_pMainRenderer)
	{
		MSG_RETURN(E_FAIL, "CMainApp::Render", "Null Exception: m_pGameInstance || m_pMainRenderer");
	}

	m_pGameInstance->Clear_BackBuffer_View(BACK_BUFFER_COLOR);
	m_pGameInstance->Clear_DepthStencil_View();

	m_pMainRenderer->Draw_RenderGroup();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Default_Settings()
{
	if (FAILED(m_pGameInstance->Add_Timer(DEFAULT_FPS)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Default_Settings", "Failed To Add Timer");
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Component_Prototype()
{
	if (FAILED(m_pGameInstance->Add_Component_Prototype(TEXT("Component:Renderer:Main"), CRenderer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: CRenderer");
	}

	m_pMainRenderer = dynamic_pointer_cast<CRenderer>(m_pGameInstance->Clone_Component(TEXT("Component:Renderer:Main")));
	if (nullptr == m_pMainRenderer)
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Clone_Component: CRenderer");
	}

	return S_OK;
}
