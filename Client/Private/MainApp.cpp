#include "ClientPCH.h"
#include "MainApp.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Scene_Load.h"
#include "Camera_Main.h"

CMainApp::CMainApp()
	: m_pGameInstance	(CGameInstance::Get_Instance())
#if ACTIVATE_IMGUI
	, m_pImGui_Manager	(CImGui_Manager::Get_Instance())
#endif
{
}

CMainApp::~CMainApp()
{
	CGameInstance::Release_Engine();

	CMainApp::Destroy_Instance();

#if ACTIVATE_IMGUI
	CImGui_Manager::Destroy_Instance();
#endif

#ifdef D3D11_LIVE_OBJECT_REF_COUNTER_CHECKER
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

	if (FAILED(m_pGameInstance->Initialize_Engine(m_pDevice, m_pContext, tGraphicDesc, SCENE::STATIC, SCENE::MAX, COLLISION_GROUP::MAX, SOUND_CHANNEL::MAX)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: m_pGameInstance->Initialize_Engine");
	}

#if	ACTIVATE_IMGUI
	if (FAILED(m_pImGui_Manager->Initialize(g_hWnd, m_pDevice, m_pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pImGui_Manager->Initialize");
	}
#endif

	if (FAILED(Default_Settings()))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: Default_Settings");
	}

	if (FAILED(Ready_Component_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed: Ready_Component_Prototype");
	}

	if (FAILED(m_pGameInstance->Load_Sound(TEXT("Bin/Resources/Sound/BGM/"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Load_Sound");
	}
	if (FAILED(m_pGameInstance->Load_Sound(TEXT("Bin/Resources/Sound/RailGunner/"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Load_Sound");
	}
	if (FAILED(m_pGameInstance->Load_Sound(TEXT("Bin/Resources/Sound/Brother/"))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Load_Sound");
	}

	if (FAILED(m_pGameInstance->Open_Scene(SCENE::LOADING, CScene_Load::Create(m_pDevice, m_pContext, SCENE::MENU))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to Open_Scene");
	}

	return S_OK;
}

void CMainApp::Tick(_float _fTimeDelta)
{
#if ACTIVATE_IMGUI
	if (m_pImGui_Manager)
	{
		m_pImGui_Manager->Tick();
	}
	if (m_pImGui_Manager->Is_Enable())
	{
		static _bool bGameStatus = true;
		ImGui_GameStatus(_fTimeDelta , &bGameStatus);
	}
#endif
	if (m_pGameInstance)
	{
		m_pGameInstance->Tick_Engine(_fTimeDelta);
	}
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pMainRenderer)
	{
		MSG_RETURN(E_FAIL, "CMainApp::Render", "Null Exception: m_pMainRenderer");
	}

	m_pGameInstance->Clear_BackBuffer_View(BACK_BUFFER_COLOR);
	m_pGameInstance->Clear_DepthStencil_View();

	m_pMainRenderer->Draw_RenderGroup();

#ifdef _DEBUG
	m_pGameInstance->Debug_Engine();
#if ACTIVATE_IMGUI
	m_pImGui_Manager->Render();
#endif
#endif

	m_pGameInstance->Present();

	return S_OK;
}

#pragma region Windows Management

LRESULT CMainApp::WndProcHandler(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	if (m_pGameInstance)
	{
		return m_pGameInstance->WndProcHandler(_hWnd, _message, _wParam, _lParam);
	}

	return 0;
}

#pragma endregion
#pragma region Timer Management

void CMainApp::Tick_Timer()
{
	if (m_pGameInstance)
	{
		m_pGameInstance->Tick_Timer();
	}
}

_bool CMainApp::Check_Timer(const _float _fFPS)
{
	if (m_pGameInstance)
	{
		return m_pGameInstance->Check_Timer(_fFPS);
	}

	return false;
}

_float CMainApp::Get_TimeDelta(const _float _fFPS) const
{
	if (m_pGameInstance)
	{
		return m_pGameInstance->Get_TimeDelta(_fFPS);
	}

	return 0.f;
}

#pragma endregion

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
	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_RENDERER_MAIN,
		m_pMainRenderer = CRenderer::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_RENDERER_MAIN");
	}

	shared_ptr<CShader> pShader;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX");
	}
	pShader->Bind_Matrix(SHADER_MATORTHOGRAPHIC, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXCUBEPOSTEX,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/CubePosTex.hlsl"), VTXCUBEPOSTEX::tElements, VTXCUBEPOSTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXCUBEPOSTEX");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTex.hlsl"), VTXPOSNORTEX::tElements, VTXPOSNORTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXMESH,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTanTex.hlsl"), VTXPOSNORTANTEX::tElements, VTXPOSNORTANTEX::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXMESH");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosNorTanTexBone.hlsl"), VTXPOSNORTANTEXBONE::tElements, VTXPOSNORTANTEXBONE::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINT,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstPoint.hlsl"), VTXPOSSIZEINSTTRANSCOLORARG::tElements, VTXPOSSIZEINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINT");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINE,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstLine.hlsl"), VTXPOSSIZEINSTTRANSCOLORARG::tElements, VTXPOSSIZEINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINE");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstRect.hlsl"), VTXPOSSIZEINSTTRANS::tElements, VTXPOSSIZEINSTTRANS::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESH,
		pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/InstMesh.hlsl"), VTXPOSNORTANTEXINSTTRANSCOLORARG::tElements, VTXPOSNORTANTEXINSTTRANSCOLORARG::iNumElement))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESH");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_RECT,
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_RECT");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_VIBUFFER_CUBE,
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_VIBUFFER_CUBE");
	}

	if (FAILED(m_pGameInstance->Add_Component_Prototype(SCENE::STATIC, PROTOTYPE_COMPONENT_COLLIDER,
		CCollider::Create(m_pDevice, m_pContext))))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Ready_Component_Prototype", "Failed to Add_Component_Prototype: PROTOTYPE_COMPONENT_COLLIDER");
	}

	return S_OK;
}

#if ACTIVATE_IMGUI
void CMainApp::ImGui_GameStatus(_float _fTimeDelta, _bool* _pOpen)
{
	static int location = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (location >= 0)
	{
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	else if (location == -2)
	{
		// Center window
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Game Status", _pOpen, window_flags))
	{
		ImGui::Text("Game Status");
		ImGui::Separator();

		ImGui::Text("FPS: %f", 1.f / _fTimeDelta);

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom",       NULL, location == -1)) location = -1;
			if (ImGui::MenuItem("Center",       NULL, location == -2)) location = -2;
			if (ImGui::MenuItem("Top-left",     NULL, location == 0)) location = 0;
			if (ImGui::MenuItem("Top-right",    NULL, location == 1)) location = 1;
			if (ImGui::MenuItem("Bottom-left",  NULL, location == 2)) location = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
			if (_pOpen && ImGui::MenuItem("Close")) *_pOpen = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}
#endif
