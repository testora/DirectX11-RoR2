#include "EnginePCH.h"
#include "Renderer.h"
#include "System.h"

CRenderer::CRenderer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::RENDERER)
{
}

HRESULT CRenderer::Initialize(any _arg)
{
	m_vecFnRenderGroup.emplace_back([this] { return this->Ready_Camera(); });
	m_vecFnRenderGroup.emplace_back([this] { return this->Render_Priority(); });
	m_vecFnRenderGroup.emplace_back([this] { return this->Render_NonBlend(); });
	m_vecFnRenderGroup.emplace_back([this] { return this->Render_Blend(); });
	m_vecFnRenderGroup.emplace_back([this] { return this->Render_UI(); });

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(const RENDER_GROUP _eRenderGroup, shared_ptr<ISystem> _pGameObject)
{
	if (nullptr == _pGameObject)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Add_RenderObject", "Nullptr Exception: _pGameObject");
	}

	m_lstRenderGroup[IDX(_eRenderGroup)].emplace_back(_pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderGroup()
{
	HRESULT hr = S_OK;

	for (auto& pFnRenderGroup : m_vecFnRenderGroup)
	{
		if (FAILED(pFnRenderGroup()))
		{
			MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_RenderGroup");
			hr = E_FAIL;
		}
	}

	return hr;
}

#pragma region RenderGroup
HRESULT CRenderer::Ready_Camera()
{
	HRESULT hr = S_OK;

	for (auto& pGameObject : m_lstRenderGroup[IDX(RENDER_GROUP::CAMERA)])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::CAMERA)].clear();

	return hr;
}

HRESULT CRenderer::Render_Priority()
{
	HRESULT hr = S_OK;

	for (auto& pGameObject : m_lstRenderGroup[IDX(RENDER_GROUP::PRIORITY)])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::PRIORITY)].clear();

	return hr;
}

HRESULT CRenderer::Render_NonBlend()
{
	HRESULT hr = S_OK;

	for (auto& pGameObject : m_lstRenderGroup[IDX(RENDER_GROUP::NONBLEND)])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::NONBLEND)].clear();

	return hr;
}

HRESULT CRenderer::Render_Blend()
{
	HRESULT hr = S_OK;

	for (auto& pGameObject : m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)].clear();

	return hr;
}

HRESULT CRenderer::Render_UI()
{
	HRESULT hr = S_OK;

	for (auto& pGameObject : m_lstRenderGroup[IDX(RENDER_GROUP::UI)])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::UI)].clear();

	return hr;
}
#pragma endregion

shared_ptr<CRenderer> CRenderer::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRenderer> pInstance = make_private_shared(CRenderer, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRenderer::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CRenderer::Clone(any)
{
	return shared_from_componenet();
}
