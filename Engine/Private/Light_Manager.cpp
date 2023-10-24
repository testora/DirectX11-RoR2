#include "EnginePCH.h"
#include "Light_Manager.h"
#include "Light.h"
#include "Shader.h"
#include "VIBuffer.h"
#include "GameObject.h"

HRESULT CLight_Manager::Initialize(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const GRAPHICDESC _tGraphicDesc, const SCENE _eSceneMax)
{
	m_pDevice	= _pDevice;
	m_pContext	= _pContext;

	m_eSceneMax	= _eSceneMax;

	m_arrLights	= Function::CreateDynamicArray<Lights>(IDX(_eSceneMax), false);

	m_vecShadowViewMatrices.reserve(1);
	m_vecShadowProjectionMatrices.reserve(1);
	m_vecShadowShaderResourceViews.reserve(1);

	m_tTexture2DDesc.Width				= _tGraphicDesc.iWinCX;
	m_tTexture2DDesc.Height				= _tGraphicDesc.iWinCY;
	m_tTexture2DDesc.MipLevels			= 1;
	m_tTexture2DDesc.ArraySize			= 1;
	m_tTexture2DDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_tTexture2DDesc.SampleDesc.Count	= 1;
	m_tTexture2DDesc.SampleDesc.Quality	= 0;

	m_tTexture2DDesc.Usage				= D3D11_USAGE_DEFAULT;
	m_tTexture2DDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_tTexture2DDesc.CPUAccessFlags		= 0;
	m_tTexture2DDesc.MiscFlags			= 0;

	return S_OK;
}

void CLight_Manager::Tick()
{
	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		for (auto iter = m_arrLights[i].begin(); iter != m_arrLights[i].end();)
		{
			if ((*iter)->Is_Expired())
			{
				iter = m_arrLights[i].erase(iter);
			}
			else
			{
				(*iter)->Tick();
				++iter;
			}
		}
	}
}

HRESULT CLight_Manager::Add_Light(const SCENE _eScene, LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform, shared_ptr<CGameObject> _pObject)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Invalid Range: SCENE");
	}

	if (m_iLightCount >= g_iMaxLights - 2)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Out of Range: m_iLightCount");
	}

	shared_ptr<CLight> pLight = CLight::Create(_tLightDesc, _pTransform);
	if (nullptr == pLight)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Failed to Create: CLight");
	}

	m_arrLights[IDX(_eScene)].emplace_back(pLight);

	++m_iLightCount;

	if (LIGHTTYPE::SHADOW == _tLightDesc.eLightType)
	{
		m_mapShadows.emplace(_pObject, pLight);
	}

    return S_OK;
}

HRESULT CLight_Manager::Clear_Lights(const SCENE _eScene)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Clear_Lights", "Invalid Range: SCENE");
	}

	m_iLightCount -= static_cast<_uint>(m_arrLights[IDX(_eScene)].size());

	m_arrLights[IDX(_eScene)].clear();

	return S_OK;
}

HRESULT CLight_Manager::Bind_Lights(shared_ptr<CShader> _pShader, shared_ptr<CVIBuffer_Rect> _pVIBuffer)
{
	HRESULT hr = S_OK;

	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		for (auto& pLight : m_arrLights[i])
		{
			if (FAILED(pLight->Bind_Light(_pShader, _pVIBuffer)))
			{
				hr = E_FAIL;
				MSG_BOX("CLight_Manager::Bind_Lights", "Failed to Bind_Light");
			}
		}
	}

    return hr;
}

HRESULT CLight_Manager::Push_ShadowResourceView(ComPtr<ID3D11ShaderResourceView> _pTarget, shared_ptr<CGameObject> _pObject)
{
	ComPtr<ID3D11Texture2D>				pTexture2D;
	ComPtr<ID3D11ShaderResourceView>	pShaderResourceView;

	if (FAILED(m_pDevice->CreateTexture2D(&m_tTexture2DDesc, nullptr, pTexture2D.GetAddressOf())))
	{
		MSG_BOX("CLight_Manager::Push_ShadowResourceView", "Failed to CreateTexture2D");
	}
	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D.Get(), nullptr, pShaderResourceView.GetAddressOf())))
	{
		MSG_BOX("CLight_Manager::Push_ShadowResourceView", "Failed to CreateShaderResourceView");
	}

	ComPtr<ID3D11Resource> pSrc, pDst;

	_pTarget->GetResource(pSrc.GetAddressOf());
	pShaderResourceView->GetResource(pDst.GetAddressOf());
	m_pContext->CopyResource(pDst.Get(), pSrc.Get());

	m_mapShadowShaderResourceViews.emplace(_pObject, pShaderResourceView);

	return S_OK;
}

HRESULT CLight_Manager::Bind_ShadowViewMatrices(shared_ptr<CShader> _pShader)
{
	m_vecShadowViewMatrices.clear();
	m_vecShadowProjectionMatrices.clear();
	m_vecShadowShaderResourceViews.clear();

	for (auto& pair : m_mapShadows)
	{
		m_vecShadowViewMatrices.emplace_back(pair.second->Get_ShadowViewMatrix());
		m_vecShadowProjectionMatrices.emplace_back(pair.second->Get_ShadowProjectionMatrix());
		m_vecShadowShaderResourceViews.emplace_back(m_mapShadowShaderResourceViews[pair.first]);
	}

	if (FAILED(_pShader->Bind_Int(SHADER_SHADOWCOUNT, static_cast<_int>(m_mapShadows.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowViewMatrices", "Failed to Bind_Int: SHADER_SHADOWCOUNT");
	}
	if (FAILED(_pShader->Bind_MatrixArray(SHADER_MATSHADOWVIEWS, m_vecShadowViewMatrices.data(), static_cast<_int>(m_vecShadowViewMatrices.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowViewMatrices", "Failed to Bind_MatrixArray: SHADER_MATSHADOWVIEWS");
	}
	if (FAILED(_pShader->Bind_MatrixArray(SHADER_MATSHADOWPROJS, m_vecShadowProjectionMatrices.data(), static_cast<_int>(m_vecShadowProjectionMatrices.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowViewMatrices", "Failed to Bind_MatrixArray: SHADER_MATSHADOWPROJS");
	}
	if (FAILED(_pShader->Bind_ShaderResourceViews(SHADER_TEXTARGET_SHADOWDEPTH,
		Function::ConvertToRawPtrVector(m_vecShadowShaderResourceViews).data(), 0, static_cast<_uint>(m_vecShadowShaderResourceViews.size()))))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Bind_ShadowViewMatrices", "Failed to Bind_ShaderResourceViews: SHADER_TEXTARGET_SHADOWDEPTH");
	}

	m_mapShadowShaderResourceViews.clear();

    return S_OK;
}

HRESULT CLight_Manager::Render_ShadowDepth(shared_ptr<CShader> _pShader, _uint _iPassIndex, shared_ptr<CVIBuffer> _pVIBuffer, shared_ptr<CGameObject> _pObject)
{
	if (m_mapShadows.end() == m_mapShadows.find(_pObject))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Render_ShadowDepth", "Invalid Object");
	}

	if (FAILED(m_mapShadows[_pObject]->Bind_ShadowMatrices(_pShader)))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Render_ShadowDepth", "Failed to Bind_ShadowMatrices");
	}

	if (FAILED(_pVIBuffer->Render(_pShader, _iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Render_ShadowDepth", "Failed to Render");
	}

	return S_OK;
}
