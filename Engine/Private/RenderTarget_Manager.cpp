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

#ifdef _DEBUG
ComPtr<ID3D11ShaderResourceView> CRenderTarget_Manager::Get_ShaderResourceView(const wstring& wstrTargetTag)
{
	if (!Exist_RenderTarget(wstrTargetTag))
	{
		MSG_RETURN(nullptr, "CRenderTarget_Manager::Get_ShaderResourceView", "Invalid RenderTarget");
	}

	return m_umapRenderTarget[wstrTargetTag]->Get_ShaderResourceView();
}
#endif

HRESULT CRenderTarget_Manager::Push_RenderTarget(const wstring& _wstrMultiTargetTag, const wstring& _wstrTargetTag)
{
	if (!Exist_RenderTarget(_wstrTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Push_RenderTarget", "Invalid RenderTarget");
	}

	m_umapMultiRenderTarget[_wstrMultiTargetTag].emplace_back(m_umapRenderTarget[_wstrTargetTag]);

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

HRESULT CRenderTarget_Manager::Begin_MultiRenderTaget(const wstring& _wstrMultiTargetTag)
{
	if (!Exist_MultiRenderTarget(_wstrMultiTargetTag))
	{
		MSG_RETURN(E_FAIL, "CRenderTarget_Manager::Begin_MultiRenderTaget", "Invalid MultiRenderTarget");
	}

	ID3D11ShaderResourceView*	pShaderResourceView[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
	ID3D11RenderTargetView*		pRenderTargetView[MAX_RENDERTARGET] = { nullptr };

	m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pShaderResourceView);
	m_pContext->OMGetRenderTargets(1, m_pBackBufferView.GetAddressOf(), m_pDepthStencilView.GetAddressOf());

	_uint iIndex(0);
	for (auto& pRenderTarget : m_umapMultiRenderTarget[_wstrMultiTargetTag])
	{
		pRenderTarget->Clear(m_pContext);
		pRenderTargetView[iIndex++] = pRenderTarget->Get_RenderTargetView().Get();
	}

	m_pContext->OMSetRenderTargets(MAX_RENDERTARGET, pRenderTargetView, m_pDepthStencilView.Get());

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

_bool CRenderTarget_Manager::Exist_RenderTarget(const wstring& _wstrTargetTag)
{
	return m_umapRenderTarget.find(_wstrTargetTag) != m_umapRenderTarget.end();
}

_bool CRenderTarget_Manager::Exist_MultiRenderTarget(const wstring& _wstrMultiTargetTag)
{
	return m_umapMultiRenderTarget.find(_wstrMultiTargetTag) != m_umapMultiRenderTarget.end();
}
