#include "ClientPCH.h"
#include "Loader.h"
#include "Thread_Loader.h"

#pragma region Scene Loader Header
#include "Loader_Menu.h"
#include "Loader_Tool.h"
#include "Loader_Test.h"
#include "Loader_Moon.h"
#pragma endregion

CLoader::CLoader(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eLoadScene)
	: m_pDevice		(_pDevice)
	, m_pContext	(_pContext)
	, m_eLoadScene	(_eLoadScene)
{
}

CLoader::~CLoader()
{
	Free();
}

HRESULT CLoader::Initialize()
{
	InitializeCriticalSection(&m_tCriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Loader_Main, this, 0, nullptr);
	if (0 == m_hThread)
	{
		MSG_RETURN(E_FAIL, "CLoader::Initialize", "Failed to _beginthreadex");
	}

	return S_OK;
}

HRESULT CLoader::Loading()
{
	HRESULT hr = { 0 };

	EnterCriticalSection(&m_tCriticalSection);

	switch (m_eLoadScene)
	{
	case SCENE::MENU:
		hr = Load_Menu();
		break;

	case SCENE::TOOL:
		hr = Load_Tool();
		break;

	case SCENE::TEST:
		hr = Load_Test();
		break;

	case SCENE::MOON:
		hr = Load_Moon();
		break;
	}

	LeaveCriticalSection(&m_tCriticalSection);

	if (FAILED(hr))
	{
		MSG_RETURN(E_FAIL, "CLoader:Loading", "Failed to Load");
	}
	else
	{
		m_bIsFinish = true;
	}

	return S_OK;
}

shared_ptr<CLoader> CLoader::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eLoadScene)
{
	shared_ptr<CLoader> pInstance = make_private_shared(CLoader, _pDevice, _pContext, _eLoadScene);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CLoader::Create", "Failed to Initialize");
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_tCriticalSection);
}
