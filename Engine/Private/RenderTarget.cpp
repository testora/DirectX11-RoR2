#include "EnginePCH.h"
#include "RenderTarget.h"
#include "Shader.h"

HRESULT CRenderTarget::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const _uint2 _vResolution, const DXGI_FORMAT _eFormat, const _color _vClear)
{
	D3D11_TEXTURE2D_DESC tTexture2DDesc{};

	tTexture2DDesc.Width				= _vResolution.x;
	tTexture2DDesc.Height				= _vResolution.y;
	tTexture2DDesc.MipLevels			= 1;
	tTexture2DDesc.ArraySize			= 1;
	tTexture2DDesc.Format				= _eFormat;

	tTexture2DDesc.SampleDesc.Count		= 1;
	tTexture2DDesc.SampleDesc.Quality	= 0;

	tTexture2DDesc.Usage				= D3D11_USAGE_DEFAULT;
	tTexture2DDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tTexture2DDesc.CPUAccessFlags		= 0;
	tTexture2DDesc.MiscFlags			= 0;

	if (FAILED(_pDevice->CreateTexture2D(&tTexture2DDesc, nullptr, m_pTexture2D.GetAddressOf())))
	{
		MSG_BOX("CRenderTarget::Initialize", "Failed to CreateTexture2D");
	}
	if (FAILED(_pDevice->CreateRenderTargetView(m_pTexture2D.Get(), nullptr, m_pRenderTargetView.GetAddressOf())))
	{
		MSG_BOX("CRenderTarget::Initialize", "Failed to CreateRenderTargetView");
	}
	if (FAILED(_pDevice->CreateShaderResourceView(m_pTexture2D.Get(), nullptr, m_pShaderResourceView.GetAddressOf())))
	{
		MSG_BOX("CRenderTarget::Initialize", "Failed to CreateShaderResourceView");
	}

	return S_OK;
}

void CRenderTarget::Clear(ComPtr<ID3D11DeviceContext> _pContext)
{
	_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<_float*>(&m_vClear));
}

HRESULT CRenderTarget::Bind_ShaderResourceView(shared_ptr<CShader> _pShader, const _char* _szConstantName)
{
	if (FAILED(_pShader->Bind_ShaderResourceView(_szConstantName, m_pShaderResourceView.Get())))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	return S_OK;
}

shared_ptr<CRenderTarget> CRenderTarget::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const _uint2 _vResolution, const DXGI_FORMAT _eFormat, const _color _vClear)
{
	shared_ptr<CRenderTarget> pInstance = make_private_shared(CRenderTarget);

	if (FAILED(pInstance->Initialize(_pDevice, _pContext, _vResolution, _eFormat, _vClear)))
	{
		MSG_BOX("CRenderTarget::Create", "Failed to Initialize");
	}

	return pInstance;
}
