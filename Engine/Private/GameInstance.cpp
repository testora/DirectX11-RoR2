#include "GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::Get_Instance())
	, m_pGraphic_Device(CGraphic_Device::Get_Instance())
{
}

#pragma region Engine

HRESULT CGameInstance::Initialize_Engine(_In_ const GRAPHICDESC _tGraphicDesc, _Out_ ComPtr<ID3D11Device>& _pDevice, _Out_ ComPtr<ID3D11DeviceContext>& _pContext)
{
	if (FAILED(m_pGraphic_Device->Ready_GraphicDevice(_tGraphicDesc, _pDevice, _pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed: m_pGraphic_Device->Ready_GraphicDevice");
	}

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{

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

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vColor)
{
	if (nullptr == m_pGraphic_Device)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_BackBuffer_View", "Null Exception: m_pGraphic_Device");
	}

	return m_pGraphic_Device->Clear_BackBuffer_View(vColor);
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

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Destroy_Instance();
	CTimer_Manager::Destroy_Instance();
	CGraphic_Device::Destroy_Instance();
}
