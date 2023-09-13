#include "EnginePCH.h"
#include "GameInstance.h"
#include "GraphicDevice.h"
#include "Timer_Manager.h"
#include "Mouse_Manager.h"
#include "Key_Manager.h"
#include "Event_Handler.h"
#include "Scene_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"
#include "Grid_Manager.h"
#include "Light_Manager.h"
#include "Picker.h"

CGameInstance::CGameInstance()
	: m_pGraphic_Device		(CGraphicDevice::Get_Instance())
	, m_pTimer_Manager		(CTimer_Manager::Get_Instance())
	, m_pMouse_Manager		(CMouse_Manager::Get_Instance())
	, m_pKey_Manager		(CKey_Manager::Get_Instance())
	, m_pEvent_Handler		(CEvent_Handler::Get_Instance())
	, m_pScene_Manager		(CScene_Manager::Get_Instance())
	, m_pObject_Manager		(CObject_Manager::Get_Instance())
	, m_pComponent_Manager	(CComponent_Manager::Get_Instance())
	, m_pBehavior_Manager	(CBehavior_Manager::Get_Instance())
	, m_pGrid_Manager		(CGrid_Manager::Get_Instance())
	, m_pLight_Manager		(CLight_Manager::Get_Instance())
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

	if (FAILED(m_pGrid_Manager->Reserve_Manager(_eMax, g_vGridSize)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pGrid_Manager->Reserve_Manager");
	}

	if (FAILED(m_pLight_Manager->Reserve_Manager(_eMax)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pLight_Manager->Reserve_Manager");
	}

#ifdef _DEBUG
#if ACTIVATE_CONSOLE
	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}
#endif
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

	m_pLight_Manager->Tick();

	m_pPicker->Tick();

	m_pScene_Manager->Late_Tick(_fTimeDelta);
	m_pObject_Manager->Late_Tick(_fTimeDelta);

	m_pLight_Manager->Late_Tick();

	m_pEvent_Handler->Tick(_fTimeDelta);
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

void CGameInstance::CheckFocus_OnMouse(_bool _bCheck)
{
	if (nullptr == m_pMouse_Manager)
	{
		MSG_RETURN(, "CGameInstance::CheckFocus_OnMouse", "Null Exception: m_pMouse_Manager");
	}

	return m_pMouse_Manager->CheckFocus(_bCheck);
}

#pragma endregion
#pragma region Key Manager

_bool CGameInstance::Key_None(_uint _iKey)
{
	if (nullptr == m_pKey_Manager)
	{
		MSG_RETURN(false, "CGameInstance::Key_Down", "Null Exception: m_pKey_Manager");
	}

	return _iKey == VK_MAX ? m_pKey_Manager->Key_None() : m_pKey_Manager->Key_None(_iKey);
}

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
#pragma region Event Handler

void CGameInstance::Register_OnTickListener(shared_ptr<void> _pSubscriber, function<_bool(_float)> _funcListener)
{
	if (nullptr == m_pEvent_Handler)
	{
		MSG_RETURN(, "CGameInstance::Register_OnTickListener", "Null Exception: m_pEvent_Handler");
	}

	return m_pEvent_Handler->Register_OnTickListener(_pSubscriber, _funcListener);
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

HRESULT CGameInstance::Add_Object_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag, shared_ptr<class CGameObject> _pPrototype)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Object_Prototype", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Prototype(_eScene, _wstrPrototypeTag, _pPrototype);
}

shared_ptr<class CGameObject> CGameInstance::Clone_GameObject(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_GameObject", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Clone_GameObject(_eScene, _wstrPrototypeTag, _arg);
}

shared_ptr<class CObjectLayer> CGameInstance::Add_Layer(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Layer(_eScene, _wstrLayerTag);
}

shared_ptr<class CObjectPool> CGameInstance::Add_Pool(const SCENE _eScene, const wstring& _wstrPoolTag, const wstring& _wstrPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Add_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Pool(_eScene, _wstrPoolTag, _wstrPrototypeTag, _iPoolSize, _arg);
}

shared_ptr<class CObjectLayer> CGameInstance::Find_Layer(const SCENE _eScene, const wstring& _wstrLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Layer(_eScene, _wstrLayerTag);
}

shared_ptr<class CObjectPool> CGameInstance::Find_Pool(const SCENE _eScene, const wstring& _wstrPoolTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Pool(_eScene, _wstrPoolTag);
}

void CGameInstance::Iterate_Layers(const SCENE _eScene, function<_bool(pair<wstring, shared_ptr<class CObjectLayer>>)> _funcCallback)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Iterate_Layers(_eScene, _funcCallback);
}

void CGameInstance::Iterate_Pools(const SCENE _eScene, function<_bool(pair<wstring, shared_ptr<class CObjectPool>>)> _funcCallback)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(, "CGameInstance::Iterate_Layers", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Iterate_Pools(_eScene, _funcCallback);
}

#pragma endregion
#pragma region Component Manager

HRESULT CGameInstance::Add_Component_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag, shared_ptr<CComponent> _pPrototype)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Component_Prototype", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Add_Prototype(_eScene, _wstrPrototypeTag, _pPrototype);
}

shared_ptr<CComponent> CGameInstance::Clone_Component(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_Component", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Clone_Component(_eScene, _wstrPrototypeTag, _arg);
}

#pragma endregion
#pragma region Behavior Manager

HRESULT CGameInstance::Add_Behavior_Prototype(const SCENE _eScene, const wstring& _wstrPrototypeTag, shared_ptr<CBehavior> _pPrototype)
{
	if (nullptr == m_pBehavior_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Behavior_Prototype", "Null Exception: m_pBehavior_Manager");
	}

	return m_pBehavior_Manager->Add_Prototype(_eScene, _wstrPrototypeTag, _pPrototype);
}

shared_ptr<CBehavior> CGameInstance::Clone_Behavior(const SCENE _eScene, const wstring& _wstrPrototypeTag, any _arg)
{
	if (nullptr == m_pBehavior_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_Component", "Null Exception: m_pBehavior_Manager");
	}

	return m_pBehavior_Manager->Clone_Behavior(_eScene, _wstrPrototypeTag, _arg);
}

#pragma endregion
#pragma region Grid Manager

_float3 CGameInstance::Get_GridSize()
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(_float3{}, "CGameInstance::Get_GridSize", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Get_GridSize();
}

HRESULT CGameInstance::Register_VIBuffer(const SCENE _eScene, const wstring& _wstrGridLayerTag, shared_ptr<class CGameObject> _pObject)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Register_VIBuffer", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Register_VIBuffer(_eScene, _wstrGridLayerTag, _pObject);
}

HRESULT CGameInstance::Reset_Grids(const SCENE _eScene, const wstring& _wstrGridLayerTag)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Reset_Grids", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Reset_Grids(_eScene, _wstrGridLayerTag);
}

_float3 CGameInstance::Raycast(_vectorf _vRayOrigin, _vectorf _vRayDirection, _float _fRange)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(_vRayOrigin, "CGameInstance::Raycast", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Raycast(_vRayOrigin, _vRayDirection, _fRange);
}

_float3 CGameInstance::Raycast(const wstring& _wstrGridLayerTag, _vectorf _vRayOrigin, _vectorf _vRayDirection, _float _fRange)
{
	if (nullptr == m_pGrid_Manager)
	{
		MSG_RETURN(_vRayOrigin, "CGameInstance::Raycast", "Null Exception: m_pGrid_Manager");
	}

	return m_pGrid_Manager->Raycast(_wstrGridLayerTag, _vRayOrigin, _vRayDirection, _fRange);
}

#pragma endregion
#pragma region Light Manager

HRESULT CGameInstance::Add_BindShaders(shared_ptr<class CShader> _pShader)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_BindShaders", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Add_Shaders(_pShader);
}

HRESULT CGameInstance::Add_Lights(const SCENE _eScene, LIGHTDESC _tLightDesc, shared_ptr<class CTransform> _pTransform)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Lights", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Add_Lights(_eScene, _tLightDesc, _pTransform);
}

HRESULT CGameInstance::Clear_Lights(const SCENE _eScene)
{
	if (nullptr == m_pLight_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_Lights", "Null Exception: m_pLight_Manager");
	}

	return m_pLight_Manager->Clear_Lights(_eScene);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Destroy_Instance();
	CPicker::Destroy_Instance();
	CLight_Manager::Destroy_Instance();
	CGrid_Manager::Destroy_Instance();
	CBehavior_Manager::Destroy_Instance();
	CComponent_Manager::Destroy_Instance();
	CObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
	CKey_Manager::Destroy_Instance();
	CMouse_Manager::Destroy_Instance();
	CTimer_Manager::Destroy_Instance();

#ifdef _DEBUG
#if ACTIVATE_CONSOLE
	FreeConsole();
#endif
#endif
}
