#include "EnginePCH.h"
#include "GameInstance.h"
#include "GraphicDevice.h"
#include "Timer_Manager.h"
#include "Mouse_Manager.h"
#include "Key_Manager.h"
#include "Scene_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"
#include "Picker.h"

CGameInstance::CGameInstance()
	: m_pGraphic_Device		(CGraphicDevice::Get_Instance())
	, m_pTimer_Manager		(CTimer_Manager::Get_Instance())
	, m_pMouse_Manager		(CMouse_Manager::Get_Instance())
	, m_pKey_Manager		(CKey_Manager::Get_Instance())
	, m_pScene_Manager		(CScene_Manager::Get_Instance())
	, m_pObject_Manager		(CObject_Manager::Get_Instance())
	, m_pComponent_Manager	(CComponent_Manager::Get_Instance())
	, m_pBehavior_Manager	(CBehavior_Manager::Get_Instance())
	, m_pPicker				(CPicker::Get_Instance())
{
}

#pragma region Engine

HRESULT CGameInstance::Initialize_Engine(_In_ const SCENE _eStatic, _In_ const SCENE _eMax, _In_ const GRAPHICDESC _tGraphicDesc, _Out_ ComPtr<ID3D11Device>& _pDevice, _Out_ ComPtr<ID3D11DeviceContext>& _pContext)
{
	if (FAILED(m_pGraphic_Device->Ready_GraphicDevice(_tGraphicDesc, _pDevice, _pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pGraphic_Device->Ready_GraphicDevice");
	}

	if (FAILED(m_pPicker->Initialize(_tGraphicDesc.hWnd, _pDevice, _pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pPicker->Initialize");
	}

	if (FAILED(m_pMouse_Manager->Initialize(_tGraphicDesc.hWnd, POINT{static_cast<LONG>(_tGraphicDesc.iWinCX), static_cast<LONG>(_tGraphicDesc.iWinCY)})))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pMouse_Manager->Initialize");
	}

	if (FAILED(m_pKey_Manager->Initialize(_tGraphicDesc.hWnd)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pKey_Manager->Initialize");
	}

	if (FAILED(m_pScene_Manager->Initialize(_eStatic, _eMax)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pScene_Manager->Initialize");
	}

	if (FAILED(m_pObject_Manager->Reserve_Manager(_eMax)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pObject_Manager->Reserve_Manager");
	}

	if (FAILED(m_pComponent_Manager->Reserve_Manager(_eMax)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pComponent_Manager->Reserve_Manager");
	}

	if (FAILED(m_pBehavior_Manager->Reserve_Manager(_eMax)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pBehavior_Manager->Reserve_Manager");
	}

#ifdef _DEBUG

	if (::AllocConsole() == TRUE && ACTIVATE_CONSOLE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif

	return S_OK;
}

void CGameInstance::Tick_Engine(_float _fTimeDelta)
{
	if (!m_pMouse_Manager
	||	!m_pKey_Manager
	||	!m_pScene_Manager
	||	!m_pObject_Manager)
	{
		MSG_RETURN(, "CGameInstance::Tick_Engine", "Null Exception");
	}

	m_pMouse_Manager->Tick(m_tWndProcDesc);
	m_pKey_Manager->Tick();

	m_pScene_Manager->Tick(_fTimeDelta);
	m_pObject_Manager->Tick(_fTimeDelta);

	m_pPicker->Tick();

	m_pScene_Manager->Late_Tick(_fTimeDelta);
	m_pObject_Manager->Late_Tick(_fTimeDelta);
}

LRESULT CGameInstance::WndProcHandler(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	m_tWndProcDesc.hWnd		= _hWnd;
	m_tWndProcDesc.message	= _message;
	m_tWndProcDesc.wParam	= _wParam;
	m_tWndProcDesc.lParam	= _lParam;

	return 1;
}

WNDPROCDESC CGameInstance::Get_WndProcDesc() const
{
	return m_tWndProcDesc;
}

#pragma endregion
#pragma region Timer Manager

void CGameInstance::Tick_Timer()
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Tick();
}

HRESULT CGameInstance::Add_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Add_Timer(_fFPS);
}

HRESULT CGameInstance::Delete_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Delete_Timer(_fFPS);
}

_bool CGameInstance::Check_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Check_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Check_Timer(_fFPS);
}

shared_ptr<class CTimer> CGameInstance::Find_Timer(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Tick_Timer", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Find_Timer(_fFPS);
}

_float CGameInstance::Get_TimeDelta(const _float _fFPS)
{
	if (nullptr == m_pTimer_Manager)
	{
		MSG_RETURN(0.f, "CGameInstance::Get_TimeDelta", "Null Exception: m_pTimer_Manager");
	}

	return m_pTimer_Manager->Get_TimeDelta(_fFPS);
}

#pragma endregion
#pragma region Mouse Manager

POINT CGameInstance::Get_CursorPos()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(POINT{}, "CGameInstance::Get_CursorPos", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Get_CursorPos();
}

POINT CGameInstance::Get_CursorMove()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(POINT{}, "CGameInstance::Get_CursorMove", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Get_CursorMove();
}

_bool CGameInstance::Is_CursorOn()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Is_CursorOn", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Is_CursorOn();
}

void CGameInstance::CheckFocus_OnMouse(_bool _bCheck)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::CheckFocus_OnMouse", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->CheckFocus(_bCheck);
}

void CGameInstance::Fix_Cursor(_bool _bFix)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::Fix_Cursor", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Fix_Cursor(_bFix);
}

void CGameInstance::Show_Cursor(_bool _bShow)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::Show_Cursor", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Show_Cursor(_bShow);
}

void CGameInstance::Toggle_Cursor()
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::Toggle_Cursor", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->Toggle_Cursor();
}

#pragma endregion
#pragma region Key Manager

_bool CGameInstance::Key_Down(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Down", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_Down() : m_pKey_Manager->Key_Down(_iKey);
}

_bool CGameInstance::Key_Hold(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Hold", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_Hold() : m_pKey_Manager->Key_Hold(_iKey);
}

_bool CGameInstance::Key_Up(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Down", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_Up() : m_pKey_Manager->Key_Up(_iKey);
}

void CGameInstance::CheckFocus_OnKeyboard(_bool _bCheck)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(, "CGameInstance::CheckFocus_OnKeyboard", "Null Exception: m_pKey_Manager");
	}

	return m_pKey_Manager->CheckFocus(_bCheck);
}

#pragma endregion
#pragma region Graphic Device

HRESULT CGameInstance::Clear_BackBuffer_View(_color _cBackBuffer)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_BackBuffer_View", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Clear_BackBuffer_View(_cBackBuffer);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_DepthStencil_View", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Present", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Present();
}

#pragma endregion
#pragma region Scene Manager

HRESULT CGameInstance::Open_Scene(SCENE _eScene, shared_ptr<class CScene> _pScene)
{
	if (nullptr == m_pScene_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Open_Scene", "Null Exception: m_pScene_Manager");
	}

	return m_pScene_Manager->Open_Scene(_eScene, _pScene);
}

const SCENE CGameInstance::Current_Scene(_bool _bSkipLoad)
{
	if (nullptr == m_pScene_Manager)
	{
		MSG_RETURN(m_pScene_Manager->Max_Scene(), "CGameInstance::Current_Scene", "Null Exception: m_pScene_Manager");
	}

	return m_pScene_Manager->Current_Scene(_bSkipLoad);
}

#pragma endregion
#pragma region Object Manager

HRESULT CGameInstance::Add_Object_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag, shared_ptr<class CGameObject> _pPrototype)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Object_Prototype", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Prototype(_eScene, _strPrototypeTag, _pPrototype);
}

shared_ptr<class CGameObject> CGameInstance::Clone_GameObject(const SCENE _eScene, const wstring& _strPrototypeTag, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_GameObject", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Clone_GameObject(_eScene, _strPrototypeTag, _arg);
}

shared_ptr<class CObjectLayer> CGameInstance::Add_Layer(const SCENE _eScene, const wstring& _strLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Layer(_eScene, _strLayerTag);
}

shared_ptr<class CObjectPool> CGameInstance::Add_Pool(const SCENE _eScene, const wstring& _strPoolTag, const wstring& _strPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Pool(_eScene, _strPoolTag, _strPrototypeTag, _iPoolSize, _arg);
}

shared_ptr<class CObjectLayer> CGameInstance::Find_Layer(const SCENE _eScene, const wstring& _strLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Layer(_eScene, _strLayerTag);
}

shared_ptr<class CObjectPool> CGameInstance::Find_Pool(const SCENE _eScene, const wstring& _strPoolTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Pool(_eScene, _strPoolTag);
}

#pragma endregion
#pragma region Component Manager

HRESULT CGameInstance::Add_Component_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag, shared_ptr<CComponent> _pPrototype)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Component_Prototype", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Add_Prototype(_eScene, _strPrototypeTag, _pPrototype);
}

shared_ptr<CComponent> CGameInstance::Clone_Component(const SCENE _eScene, const wstring& _strPrototypeTag, any _arg)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_Component", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Clone_Component(_eScene, _strPrototypeTag, _arg);
}

#pragma endregion
#pragma region Behavior Manager

HRESULT CGameInstance::Add_Behavior_Prototype(const SCENE _eScene, const wstring& _strPrototypeTag, shared_ptr<CBehavior> _pPrototype)
{
	if (nullptr == m_pBehavior_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Behavior_Prototype", "Null Exception: m_pBehavior_Manager");
	}

	return m_pBehavior_Manager->Add_Prototype(_eScene, _strPrototypeTag, _pPrototype);
}

shared_ptr<CBehavior> CGameInstance::Clone_Behavior(const SCENE _eScene, const wstring& _strPrototypeTag, any _arg)
{
	if (nullptr == m_pBehavior_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_Component", "Null Exception: m_pBehavior_Manager");
	}

	return m_pBehavior_Manager->Clone_Behavior(_eScene, _strPrototypeTag, _arg);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Destroy_Instance();
	CPicker::Destroy_Instance();
	CBehavior_Manager::Destroy_Instance();
	CComponent_Manager::Destroy_Instance();
	CObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
	CKey_Manager::Destroy_Instance();
	CMouse_Manager::Destroy_Instance();
	CTimer_Manager::Destroy_Instance();

#ifdef _DEBUG
	if (ACTIVATE_CONSOLE)	FreeConsole();
#endif
}
