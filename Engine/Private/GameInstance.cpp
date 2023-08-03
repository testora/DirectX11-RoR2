#include "EnginePCH.h"
#include "GameInstance.h"
#include "Timer_Manager.h"
#include "GraphicDevice.h"
#include "Scene_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Behavior_Manager.h"

CGameInstance::CGameInstance()
	: m_pTimer_Manager		(CTimer_Manager::Get_Instance())
	, m_pGraphic_Device		(CGraphicDevice::Get_Instance())
	, m_pScene_Manager		(CScene_Manager::Get_Instance())
	, m_pObject_Manager		(CObject_Manager::Get_Instance())
	, m_pComponent_Manager	(CComponent_Manager::Get_Instance())
	, m_pBehavior_Manager	(CBehavior_Manager::Get_Instance())
{
}

#pragma region Engine

HRESULT CGameInstance::Initialize_Engine(_In_ const GRAPHICDESC _tGraphicDesc, _Out_ ComPtr<ID3D11Device>& _pDevice, _Out_ ComPtr<ID3D11DeviceContext>& _pContext)
{
	if (FAILED(m_pGraphic_Device->Ready_GraphicDevice(_tGraphicDesc, _pDevice, _pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pGraphic_Device->Ready_GraphicDevice");
	}

#ifdef _DEBUG

	if (ACTIVATE_CONSOLE && ::AllocConsole() == TRUE)
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
	if (!m_pScene_Manager || !m_pObject_Manager)
	{
		MSG_RETURN(, "CGameInstance::Tick_Engine", "Null Exception: m_pScene_Manager || m_pObject_Manager");
	}

	m_pScene_Manager->Tick(_fTimeDelta);
	m_pObject_Manager->Tick(_fTimeDelta);

	m_pScene_Manager->Late_Tick(_fTimeDelta);
	m_pObject_Manager->Late_Tick(_fTimeDelta);
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
#pragma region Graphic Device

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 _vColor)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_BackBuffer_View", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Clear_BackBuffer_View(_vColor);
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

#pragma endregion
#pragma region Object Manager

HRESULT CGameInstance::Add_Object_Prototype(const wstring& _strPrototypeTag, shared_ptr<class CGameObject> _pPrototype)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Object_Prototype", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Prototype(_strPrototypeTag, _pPrototype);
}

shared_ptr<class CGameObject> CGameInstance::Clone_GameObject(const wstring& _strPrototypeTag, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_GameObject", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Clone_GameObject(_strPrototypeTag, _arg);
}

HRESULT CGameInstance::Add_Layer(const wstring& _strLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Layer(_strLayerTag);
}

HRESULT CGameInstance::Add_Pool(const wstring& _strPoolTag, const wstring& _strPrototypeTag, _uint _iPoolSize, any _arg)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Add_Pool(_strPoolTag, _strPrototypeTag, _iPoolSize, _arg);
}

shared_ptr<class CObjectLayer> CGameInstance::Find_Layer(const wstring& _strLayerTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Layer", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Layer(_strLayerTag);
}

shared_ptr<class CObjectPool> CGameInstance::Find_Pool(const wstring& _strPoolTag)
{
	if (nullptr == m_pObject_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Find_Pool", "Null Exception: m_pObject_Manager");
	}

	return m_pObject_Manager->Find_Pool(_strPoolTag);
}

#pragma endregion
#pragma region Component Manager

HRESULT CGameInstance::Add_Component_Prototype(const wstring& _strPrototypeTag, shared_ptr<CComponent> _pPrototype)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Component_Prototype", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Add_Prototype(_strPrototypeTag, _pPrototype);
}

shared_ptr<CComponent> CGameInstance::Clone_Component(const wstring& _strPrototypeTag, any _arg)
{
	if (nullptr == m_pComponent_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_Component", "Null Exception: m_pComponent_Manager");
	}

	return m_pComponent_Manager->Clone_Component(_strPrototypeTag, _arg);
}

#pragma endregion
#pragma region Behavior Manager

HRESULT CGameInstance::Add_Behavior_Prototype(const wstring& _strPrototypeTag, shared_ptr<CBehavior> _pPrototype)
{
	if (nullptr == m_pBehavior_Manager)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Add_Behavior_Prototype", "Null Exception: m_pBehavior_Manager");
	}

	return m_pBehavior_Manager->Add_Prototype(_strPrototypeTag, _pPrototype);
}

shared_ptr<CBehavior> CGameInstance::Clone_Behavior(const wstring& _strPrototypeTag, any _arg)
{
	if (nullptr == m_pBehavior_Manager)
	{
		MSG_RETURN(nullptr, "CGameInstance::Clone_Component", "Null Exception: m_pBehavior_Manager");
	}

	return m_pBehavior_Manager->Clone_Behavior(_strPrototypeTag, _arg);
}

#pragma endregion

void CGameInstance::Release_Engine()
{
#ifdef _DEBUG
	if (ACTIVATE_CONSOLE)
	{
		FreeConsole();
	}
#endif

	CGameInstance::Destroy_Instance();
	CBehavior_Manager::Destroy_Instance();
	CComponent_Manager::Destroy_Instance();
	CObject_Manager::Destroy_Instance();
	CScene_Manager::Destroy_Instance();
	CTimer_Manager::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
}
