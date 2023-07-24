#include "pch.h"
#include "MainApp.h"
#include "GameInstance.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
}

CMainApp::~CMainApp()
{
//	CMainApp::Destroy_Instance();
//	CGameInstance::Release_Engine();
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC tGraphicDesc{};

	tGraphicDesc.eWinMode	= GRAPHICDESC::WINDOW;
	tGraphicDesc.hWnd		= g_hWnd;
	tGraphicDesc.iWinCX		= g_iWinCX;
	tGraphicDesc.iWinCY		= g_iWinCY;

	if (FAILED(m_pGameInstance->Initialize_Engine(tGraphicDesc, m_pDevice, m_pContext)))
	{
		MSG_RETURN(E_FAIL, "CMainApp::Initialize", "Failed to m_pGameInstance->Initialize_Engine");
	}

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{

}

HRESULT CMainApp::Render()
{
	return S_OK;
}
