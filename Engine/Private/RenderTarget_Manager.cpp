#include "EnginePCH.h"
#include "RenderTarget_Manager.h"
#include "RenderTarget.h"
#include "Shader.h"

HRESULT CRenderTarget_Manager::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	if (nullptr == _pDevice
	||	nullptr == _pContext)
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Initialize", "Nullptr Exception");
	}

	m_pDevice	= _pDevice;
	m_pContext	= _pContext;

	return S_OK;
}

ComPtr<ID3D11ShaderResourceView> CRenderTarget_Manager::Get_ShaderResourceView(const wstring& wstrTargetTag)
{
	if (!Exist_RenderTarget(wstrTargetTag))
	{
		MSG_RETURN(nullptr, "CRenderTarget_Manager::Get_ShaderResourceView", "Invalid RenderTarget");
	}

	return m_umapRenderTarget[wstrTargetTag]->Get_ShaderResourceView();
}

HRESULT CRenderTarget_Manager::Push_RenderTarget(const wstring& _wstrMultiTargetTag, const wstring& _wstrTargetTag)
{
	if (!Exist_RenderTarget(_wstrTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Push_RenderTarget", "Invalid RenderTarget");
	}

	m_umapMultiRenderTarget[_wstrMultiTargetTag].emplace_back(m_umapRenderTarget[_wstrTargetTag]);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Push_DepthStencil(const wstring& _wstrMultiTargetTag, const wstring& _wstrDepthStencilTag)
{
	if (!Exist_MultiRenderTarget(_wstrMultiTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Push_DepthStencil", "Invalid MultiRenderTarget");
	}

	if (!Exist_DepthStencil(_wstrDepthStencilTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Push_DepthStencil", "Invalid DepthStencil");
	}

	m_umapMultiDepthStencil[_wstrMultiTargetTag] = m_umapDepthStencil[_wstrDepthStencilTag];

	return S_OK;
}

HRESULT CRenderTarget_Manager::Erase_RenderTarget(const wstring& _wstrMultiTargetTag, const wstring& _wstrTargetTag)
{
	if (!Exist_MultiRenderTarget(_wstrMultiTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Erase_RenderTarget", "Invalid MultiRenderTarget");
	}

	if (!Exist_RenderTarget(_wstrTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Erase_RenderTarget", "Invalid RenderTarget");
	}

	auto iter = std::find(m_umapMultiRenderTarget[_wstrMultiTargetTag].begin(), m_umapMultiRenderTarget[_wstrMultiTargetTag].end(), m_umapRenderTarget[_wstrTargetTag]);
	if (iter == m_umapMultiRenderTarget[_wstrMultiTargetTag].end())
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Erase_RenderTarget", "Invalid RenderTarget");
	}

	m_umapMultiRenderTarget[_wstrMultiTargetTag].erase(iter);

	if (m_umapMultiRenderTarget.empty())
	{
		m_umapMultiRenderTarget.erase(_wstrMultiTargetTag);
	}

	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_RenderTarget(const wstring& _wstrTargetTag, const _uint2 _vResolution, const DXGI_FORMAT _eFormat, const _color _vClear)
{
	if (Exist_RenderTarget(_wstrTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Add_RenderTarget", "Already Exists: RenderTarget");
	}

	m_umapRenderTarget.emplace(_wstrTargetTag, CRenderTarget::Create(m_pDevice, m_pContext, _vResolution, _eFormat, _vClear));

	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_DepthStencil(const wstring& _wstrDepthStencilTag, const _uint2 _vResolution)
{
	if (Exist_DepthStencil(_wstrDepthStencilTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Add_DepthStencil", "Already Exists: DepthStencil");
	}

	ComPtr<ID3D11Texture2D>				pTexture2D;
	ComPtr<ID3D11DepthStencilView>		pDepthStencilView;
	D3D11_TEXTURE2D_DESC				tTexture2DDesc{};

	tTexture2DDesc.Width				= _vResolution.x;
	tTexture2DDesc.Height				= _vResolution.y;
	tTexture2DDesc.MipLevels			= 1;
	tTexture2DDesc.ArraySize			= 1;
	tTexture2DDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;

	tTexture2DDesc.SampleDesc.Count		= 1;
	tTexture2DDesc.SampleDesc.Quality	= 0;

	tTexture2DDesc.Usage				= D3D11_USAGE_DEFAULT;
	tTexture2DDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	tTexture2DDesc.CPUAccessFlags		= 0;
	tTexture2DDesc.MiscFlags			= 0;

	if (FAILED(m_pDevice->CreateTexture2D(&tTexture2DDesc, nullptr, pTexture2D.GetAddressOf())))
	{
		MSG_BOX("CRenderTarget::Initialize", "Failed to CreateTexture2D");
	}
	if (FAILED(m_pDevice->CreateDepthStencilView(pTexture2D.Get(), nullptr, pDepthStencilView.GetAddressOf())))
	{
		MSG_BOX("CRenderTarget::Initialize", "Failed to CreateRenderTargetView");
	}

	m_umapDepthStencil.emplace(_wstrDepthStencilTag, pDepthStencilView);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_MultiRenderTaget(const wstring& _wstrMultiTargetTag, _uint _iCopyResource)
{
	if (!Exist_MultiRenderTarget(_wstrMultiTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Begin_MultiRenderTaget", "Invalid MultiRenderTarget");
	}

	ID3D11ShaderResourceView*	pShaderResourceView[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]	= { nullptr };
	ID3D11RenderTargetView*		pRenderTargetView[MAX_RENDERTARGET]									= { nullptr };

	m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pShaderResourceView);
	m_pContext->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	_uint iIndex(0);
	for (auto& pRenderTarget : m_umapMultiRenderTarget[_wstrMultiTargetTag])
	{
		pRenderTarget->Clear(m_pContext);
		pRenderTargetView[iIndex++] = pRenderTarget->Get_RenderTargetView().Get();
	}

	if (Function::InRange(_iCopyResource, 0u, iIndex))
	{
		ComPtr<ID3D11Resource> pSrc, pDst;
		m_pBackBufferView->GetResource(pSrc.GetAddressOf());
		pRenderTargetView[_iCopyResource]->GetResource(pDst.GetAddressOf());
		m_pContext->CopyResource(pDst.Get(), pSrc.Get());
	}

	if (Exist_MultiDepthStencil(_wstrMultiTargetTag))
	{
		m_pContext->ClearDepthStencilView(m_umapMultiDepthStencil[_wstrMultiTargetTag].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_pContext->OMSetRenderTargets(MAX_RENDERTARGET, pRenderTargetView, m_umapMultiDepthStencil[_wstrMultiTargetTag].Get());
	}
	else
	{
		m_pContext->OMSetRenderTargets(MAX_RENDERTARGET, pRenderTargetView, m_pDepthStencilView.Get());
	}


	return S_OK;
}

HRESULT CRenderTarget_Manager::End_MultiRenderTarget()
{
	ID3D11RenderTargetView* pRenderTargetView[MAX_RENDERTARGET] = { nullptr };

	m_pContext->OMSetRenderTargets(MAX_RENDERTARGET, pRenderTargetView, nullptr);

	m_pContext->OMSetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.Get());

	m_pBackBufferView->Release();
	m_pDepthStencilView->Release();

	return S_OK;
}

HRESULT CRenderTarget_Manager::Bind_RenderTarget(const wstring& _wstrTargetTag, shared_ptr<CShader> _pShader, const _char* _szConstantName)
{
	if (nullptr == _pShader)
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Bind_RenderTarget", "Nullptr Exception: _pShader");
	}

	if (!Exist_RenderTarget(_wstrTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Bind_RenderTarget", "Invalid RenderTarget");
	}

	if (FAILED(m_umapRenderTarget[_wstrTargetTag]->Bind_ShaderResourceView(_pShader, _szConstantName)))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Bind_RenderTarget", "Failed to Bind_ShaderResourceView");
	}

	return S_OK;
}

inline _bool CRenderTarget_Manager::Exist_MultiRenderTarget(const wstring& _wstrMultiTargetTag)
{
	return m_umapMultiRenderTarget.find(_wstrMultiTargetTag) != m_umapMultiRenderTarget.end();
}

inline _bool CRenderTarget_Manager::Exist_MultiDepthStencil(const wstring& _wstrMultiDepthStencilTag)
{
	return m_umapMultiDepthStencil.find(_wstrMultiDepthStencilTag) != m_umapMultiDepthStencil.end();
}

inline _bool CRenderTarget_Manager::Exist_RenderTarget(const wstring& _wstrTargetTag)
{
	return m_umapRenderTarget.find(_wstrTargetTag) != m_umapRenderTarget.end();
}

inline _bool CRenderTarget_Manager::Exist_DepthStencil(const wstring& _wstrDepthStencilTag)
{
	return m_umapDepthStencil.find(_wstrDepthStencilTag) != m_umapDepthStencil.end();
}