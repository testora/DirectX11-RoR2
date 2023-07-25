#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CGraphic_Device final : public CSingleton<CGraphic_Device>
{
	friend class CSingleton<CGraphic_Device>;
private:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() DEFAULT;

public:
	HRESULT							Ready_GraphicDevice(_In_ GRAPHICDESC, _Out_ ComPtr<ID3D11Device>&, _Out_ ComPtr<ID3D11DeviceContext>&);

	HRESULT							Clear_BackBuffer_View(_float4 vColor);
	HRESULT							Clear_DepthStencil_View();
	HRESULT							Present();

private:
	HRESULT							Ready_SwapChain(GRAPHICDESC);
	HRESULT							Ready_RenderTargetView();
	HRESULT							Ready_DepthStencilView(_uint iWinCX, _uint iWinCY);

private:
	ComPtr<ID3D11Device>			m_pDevice;
	ComPtr<ID3D11DeviceContext>		m_pContext;

	ComPtr<IDXGISwapChain>			m_pSwapChain;
	ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;
};

END
