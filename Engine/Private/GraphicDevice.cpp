#include "GraphicDevice.h"

CGraphicDevice::CGraphicDevice()
{
}

HRESULT CGraphicDevice::Ready_GraphicDevice(_In_ GRAPHICDESC _tGraphicDesc, _Out_ ComPtr<ID3D11Device>& _pDevice, _Out_ ComPtr<ID3D11DeviceContext>& _pContext)
{
	_uint				iFlag = 0;
	D3D_FEATURE_LEVEL	eFeatureLevel;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, m_pDevice.GetAddressOf(), &eFeatureLevel, m_pContext.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Ready_GraphicDevice", "Failed to D3D11CreateDevice");
	}

	if (FAILED(Ready_SwapChain(_tGraphicDesc)))
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Ready_GraphicDevice", "Failed to Ready_SwapChain");
	}

	if (FAILED(Ready_RenderTargetView()))
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Ready_GraphicDevice", "Failed to Ready_RenderTargetView");
	}

	if (FAILED(Ready_DepthStencilView(_tGraphicDesc.iWinCX, _tGraphicDesc.iWinCY)))
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Ready_GraphicDevice", "Failed to Ready_DepthStencilView");
	}

	ID3D11RenderTargetView* pArrRenderTargetView[] =
	{
		m_pRenderTargetView.Get()
	};

	m_pContext->OMSetRenderTargets(sizeof(pArrRenderTargetView) / sizeof(ID3D11RenderTargetView*), pArrRenderTargetView, m_pDepthStencilView.Get());

	D3D11_VIEWPORT tViewPort{};

	tViewPort.TopLeftX = 0;
	tViewPort.TopLeftY = 0;
	tViewPort.Width = (_float)_tGraphicDesc.iWinCX;
	tViewPort.Height = (_float)_tGraphicDesc.iWinCY;
	tViewPort.MinDepth = 0.f;
	tViewPort.MaxDepth = 1.f;

	m_pContext->RSSetViewports(sizeof(pArrRenderTargetView) / sizeof(ID3D11RenderTargetView*), &tViewPort);

	_pDevice	= m_pDevice.Get();
	_pContext	= m_pContext.Get();

	return S_OK;
}

HRESULT CGraphicDevice::Clear_BackBuffer_View(_float4 _vColor)
{
	if (nullptr == m_pRenderTargetView)
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Clear_BackBuffer_View", "Nullptr Exception: m_pRenderTargetView");
	}

	m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<_float*>(&_vColor));

	return S_OK;
}

HRESULT CGraphicDevice::Clear_DepthStencil_View()
{
	if (nullptr == m_pDepthStencilView)
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Clear_DepthStencil_View", "Nullptr Exception: m_pDepthStencilView");
	}

	m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphicDevice::Present()
{
	if (nullptr == m_pSwapChain)
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Present", "Nullptr Exception: m_pSwapChain");
	}

	if (FAILED(m_pSwapChain->Present(0, 0)))
	{
		MSG_RETURN(E_FAIL, "CGraphicDevice::Present", "Failed to m_pSwapChain->Present(0, 0)");
	}

	return S_OK;
}

HRESULT CGraphicDevice::Ready_SwapChain(GRAPHICDESC _tGraphicDesc)
{
	ComPtr<IDXGIDevice>	pDXGIDevice;
	if (FAILED(m_pDevice->QueryInterface(_uuidof(IDXGIDevice), reinterpret_cast<void**>(pDXGIDevice.GetAddressOf()))))
	{
		return E_FAIL;
	}

	ComPtr<IDXGIAdapter> pDXGIAdapter;
	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(pDXGIAdapter.GetAddressOf()))))
	{
		return E_FAIL;
	}

	ComPtr<IDXGIFactory> pIDXGIFactory;
	if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pIDXGIFactory.GetAddressOf()))))
	{
		return E_FAIL;
	}

	DXGI_SWAP_CHAIN_DESC tSwapChainDesc{};

	tSwapChainDesc.BufferDesc.Width						= _tGraphicDesc.iWinCX;
	tSwapChainDesc.BufferDesc.Height					= _tGraphicDesc.iWinCY;

	tSwapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	tSwapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;

	tSwapChainDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	tSwapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	tSwapChainDesc.SampleDesc.Count						= 1;
	tSwapChainDesc.SampleDesc.Quality					= 0;
	
	tSwapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapChainDesc.BufferCount							= 1;

	tSwapChainDesc.OutputWindow							= _tGraphicDesc.hWnd;
	tSwapChainDesc.Windowed								= _tGraphicDesc.eWinMode;
	tSwapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	tSwapChainDesc.Flags								= 0;

	if (FAILED(pIDXGIFactory->CreateSwapChain(m_pDevice.Get(), &tSwapChainDesc, m_pSwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;

	// flip-model counterparts
}

HRESULT CGraphicDevice::Ready_RenderTargetView()
{
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf()))))
	{
		return E_FAIL;
	}
	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CGraphicDevice::Ready_DepthStencilView(_uint _iWinCX, _uint _iWinCY)
{
	ComPtr<ID3D11Texture2D> pDepthStencilBuffer;

	D3D11_TEXTURE2D_DESC tTextureDesc{};

	tTextureDesc.Width				= _iWinCX;
	tTextureDesc.Height				= _iWinCY;

	tTextureDesc.MipLevels			= 1;
	tTextureDesc.ArraySize			= 1;
	tTextureDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;

	tTextureDesc.SampleDesc.Count	= 1;
	tTextureDesc.SampleDesc.Quality = 0;

	tTextureDesc.Usage				= D3D11_USAGE_DEFAULT;

	tTextureDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	tTextureDesc.CPUAccessFlags		= 0;
	tTextureDesc.MiscFlags			= 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tTextureDesc, nullptr, pDepthStencilBuffer.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}
