#include "GameInstance.h"
#include "GraphicDevice.h"

CGameInstance::CGameInstance()
	: m_pGraphicDevice(CGraphicDevice::Get_Instance())
{
}

#pragma region Engine

HRESULT CGameInstance::Initialize_Engine(_In_ const GRAPHICDESC _tGraphicDesc, _Out_ ComPtr<ID3D11Device>& _pDevice, ComPtr<ID3D11DeviceContext>& _pContext)
{
	if (FAILED(m_pGraphicDevice->Ready_GraphicDevice(_tGraphicDesc, _pDevice, _pContext)))
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Initialize_Engine", "Failed to m_pGraphicDevice->Ready_GraphicDevice");
	}

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{

}

#pragma endregion
#pragma region GraphicDevice

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vColor)
{
	if (nullptr == m_pGraphicDevice)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_BackBuffer_View", "Null Exception: m_pGraphicDevice");
	}

	return m_pGraphicDevice->Clear_BackBuffer_View(vColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphicDevice)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Clear_DepthStencil_View", "Null Exception: m_pGraphicDevice");
	}

	return m_pGraphicDevice->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphicDevice)
	{
		MSG_RETURN(E_FAIL, "CGameInstance::Present", "Null Exception: m_pGraphicDevice");
	}

	return m_pGraphicDevice->Clear_DepthStencil_View();
}

#pragma endregion

void CGameInstance::Release_Engine()
{
	CGameInstance::Destroy_Instance();
	CGraphicDevice::Destroy_Instance();
}
