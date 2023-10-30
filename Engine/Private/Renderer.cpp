#include "EnginePCH.h"
#include "Renderer.h"
#include "GameInstance.h"
#include "RenderTarget_Manager.h"
#include "Light_Manager.h"
#include "Component_Manager.h"
#include "Scene_Manager.h"
#include "PipeLine.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameObject.h"
#include "Transform.h"
#include "System.h"
#include "Camera.h"

CRenderer::CRenderer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent				(_pDevice, _pContext, COMPONENT::RENDERER)
	, m_pRenderTarget_Manager	(CRenderTarget_Manager::Get_Instance())
	, m_pLight_Manager			(CLight_Manager::Get_Instance())
{
}

HRESULT CRenderer::Initialize(any)
{
	_uint			iNumViewPorts(1);
	D3D11_VIEWPORT	tViewPortDesc{};

	m_pContext->RSGetViewports(&iNumViewPorts, &tViewPortDesc);

	m_mQuadWorld		= XMMatrixScaling(tViewPortDesc.Width, tViewPortDesc.Height, 1.f);
	m_mQuadView			= XMMatrixIdentity();
	m_mQuadProjection	= XMMatrixOrthographicLH(tViewPortDesc.Width, tViewPortDesc.Height, 0.f, 1.f);

	_uint2			vResolution			= _uint2(static_cast<_uint>(tViewPortDesc.Width), static_cast<_uint>(tViewPortDesc.Height));
	_uint2			vShadowResolution	= _uint2(vResolution.x * g_iShadowScale, vResolution.y * g_iShadowScale);

#pragma region Renger Targets

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_SHADOWDEPTH,
		vShadowResolution, DXGI_FORMAT_R32G32B32A32_FLOAT, _color(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_SHADOWDEPTH");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_DIFFUSE,
		vResolution, DXGI_FORMAT_R8G8B8A8_UNORM, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_AMBIENT,
		vResolution, DXGI_FORMAT_R8G8B8A8_UNORM, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_SPECULAR,
		vResolution, DXGI_FORMAT_R8G8B8A8_UNORM, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_SPECULAR");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MATERIAL_EMISSIVE,
		vResolution, DXGI_FORMAT_R8G8B8A8_UNORM, _color(1.f, 1.f, 1.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MATERIAL_EMISSIVE");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_NORMAL,
		vResolution, DXGI_FORMAT_R16G16B16A16_UNORM, _color(1.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_NORMAL");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_DEPTH,
		vResolution, DXGI_FORMAT_R32G32B32A32_FLOAT, _color(0.f, 1.f, 1.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_DEPTH");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_LIGHT_DIFFUSE,
		vResolution, DXGI_FORMAT_R16G16B16A16_UNORM, _color(0.f, 0.f, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_LIGHT_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_LIGHT_AMBIENT,
		vResolution, DXGI_FORMAT_R16G16B16A16_UNORM, _color(0.f, 0.f, 0.f, 1.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_LIGHT_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_LIGHT_SPECULAR,
		vResolution, DXGI_FORMAT_R16G16B16A16_UNORM, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_LIGHT_SPECULAR");
	}
	
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_PREPROCESS,
		vResolution, DXGI_FORMAT_R8G8B8A8_UNORM, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_PREPROCESS");
	}
	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_MASK,
		vResolution, DXGI_FORMAT_R8G8B8A8_UNORM, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_MASK");
	}

	if (FAILED(m_pRenderTarget_Manager->Add_RenderTarget(RENDERTARGET_POSTPROCESS,
		vResolution, DXGI_FORMAT_R8G8B8A8_UNORM, _color(0.f, 0.f, 0.f, 0.f))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_RenderTarget: RENDERTARGET_POSTPROCESS");
	}

#pragma endregion
#pragma region Depth Stencils

	if (FAILED(m_pRenderTarget_Manager->Add_DepthStencil(DEPTHSTENCIL_SHADOWDEPTH, vShadowResolution)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Add_DepthStencil: DEPTHSTENCIL_SHADOWDEPTH");
	}

#pragma endregion
#pragma region Multi Render Targets

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_SHADOWDEPTH, RENDERTARGET_SHADOWDEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_DepthStencil(MULTIRENDERTARGET_SHADOWDEPTH, DEPTHSTENCIL_SHADOWDEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_DepthStencil: MULTIRENDERTARGET_NONBLEND");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_AMBIENT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_SPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_MATERIAL_EMISSIVE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_NORMAL)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_NONBLEND, RENDERTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_NONBLEND");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_LIGHT, RENDERTARGET_LIGHT_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_LIGHT");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_LIGHT, RENDERTARGET_LIGHT_AMBIENT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_LIGHT");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_LIGHT, RENDERTARGET_LIGHT_SPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_LIGHT");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_PREPROCESS, RENDERTARGET_PREPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_PREPROCESS");
	}
	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_PREPROCESS, RENDERTARGET_MASK)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_PREPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Push_RenderTarget(MULTIRENDERTARGET_POSTPROCESS, RENDERTARGET_POSTPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Push_RenderTarget: MULTIRENDERTARGET_POSTPROCESS");
	}

#pragma endregion

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/PosTex_Deferred.hlsl"), VTXPOSTEX::tElements, VTXPOSTEX::iNumElement);
	if(nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Compile Shader");
	}

	CComponent_Manager::Get_Instance()->Add_Prototype(CScene_Manager::Get_Instance()->Static_Scene(), PROTOTYPE_COMPONENT_SHADER_DEFERRED, m_pShader);

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Create VIBuffer");
	}

#pragma region

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATQUADWORLD, m_mQuadWorld)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADWORLD");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATQUADVIEW, m_mQuadView)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADVIEW");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATQUADPROJ, m_mQuadProjection)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Initialize", "Failed to Bind_Matrix: SHADER_MATQUADPROJ");
	}

#pragma endregion

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

	if (FAILED(m_pShader->Bind_Float(SHADER_TIME, CGameInstance::Get_Instance()->Get_ActivatedTime())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_RenderGroup", "Failed to Bind_Float: SHADER_TIME");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_RenderGroup", "Failed to Bind_Matrix: SHADER_MATVIEW");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_RenderGroup", "Failed to Bind_Matrix: SHADER_MATPROJ");
	}

	if (FAILED(Ready_Camera()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Ready_Camera");
	}
	if (FAILED(Render_Priority()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Priority");
	}

	if (FAILED(Render_Shadow()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Shadow");
	}
	if (FAILED(Render_NonBlend()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_NonBlend");
	}

	if (FAILED(Draw_Light()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_Light");
	}

	if (FAILED(Draw_PreProcess()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_PreProcess");
	}

	if (FAILED(Render_NonLight()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_NonLight");
	}
	if (FAILED(Render_Blend()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Blend");
	}

	if (FAILED(Draw_PostProcess()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Draw_PostProcess");
	}

	if (FAILED(Render_UI()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_UI");
	}

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
	{
		hr = E_FAIL;
		MSG_BOX("CRenderer::Draw_RenderGroup", "Failed to Render_Debug");
	}
#endif

	return hr;
}

#pragma region RenderGroup
HRESULT CRenderer::Ready_Camera()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::CAMERA)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
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

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::PRIORITY)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::PRIORITY)].clear();

	return hr;
}

HRESULT CRenderer::Render_Shadow()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::SHADOW)])
	{
		if (shared_ptr<CGameObject> pObject = dynamic_pointer_cast<CGameObject>(pSystem))
		{
			if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_SHADOWDEPTH)))
			{
				MSG_RETURN(E_FAIL, "CRenderer::Render_Shadow", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_SHADOWDEPTH");
			}

			if (FAILED(pObject->Render_ShadowDepth()))
			{
				hr = E_FAIL;
			}

			if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
			{
				MSG_RETURN(E_FAIL, "CRenderer::Render_Shadow", "Failed to End_MultiRenderTarget");
			}

			if (FAILED(m_pLight_Manager->Push_ShadowResourceView(m_pRenderTarget_Manager->Get_ShaderResourceView(RENDERTARGET_SHADOWDEPTH), pObject)))
			{
				MSG_RETURN(E_FAIL, "CRenderer::Render_Shadow", "Failed to Push_ShadowResourceView");
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::SHADOW)].clear();

	return hr;
}

HRESULT CRenderer::Render_NonBlend()
{
	HRESULT hr = S_OK;

	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_NONBLEND)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_NonBlend", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_NONBLEND");
	}

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::NONBLEND)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::NONBLEND)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_NonBlend", "Failed to End_MultiRenderTarget");
	}

	return hr;
}

HRESULT CRenderer::Draw_Light()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_LIGHT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_LIGHT");
	}

	if (CPipeLine::Get_Instance()->Get_Camera())
	{
		if (FAILED(m_pShader->Bind_Vector(SHADER_CAMPOS, CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION))))
		{
			MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Vector: SHADER_CAMPOS");
		}
		if (FAILED(m_pShader->Bind_Float(SHADER_CAMFAR, CPipeLine::Get_Instance()->Get_Camera()->Get_Desc().fFar)))
		{
			MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Float: SHADER_CAMFAR");
		}
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEWINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Matrix: SHADER_MATVIEWINV");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Matrix: SHADER_MATPROJINV");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_NORMAL, m_pShader, SHADER_TEXTARGET_NORMAL)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_NORMAL");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_DEPTH, m_pShader, SHADER_TEXTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_RenderTarget: RENDERTARGET_DEPTH");
	}

	if (FAILED(m_pLight_Manager->Bind_Lights(m_pShader, m_pVIBuffer)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to Bind_Lights");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_Light", "Failed to End_MultiRenderTarget");
	}

	return S_OK;
}

HRESULT CRenderer::Draw_PreProcess()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_PREPROCESS, 0)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_PREPROCESS");
	}

	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEWINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_Matrix: SHADER_MATVIEWINV");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJINV, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION).inverse())))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_Matrix: SHADER_MATPROJINV");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_DEPTH, m_pShader, SHADER_TEXTARGET_DEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_DEPTH");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_SHADOWDEPTH, m_pShader, SHADER_TEXTARGET_SHADOWDEPTH)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_SHADOWDEPTH");
	}

	if (FAILED(m_pLight_Manager->Bind_ShadowViewMatrices(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_ShadowViewMatrices");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_DIFFUSE, m_pShader, SHADER_TEXTARGET_MTRL_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_AMBIENT, m_pShader, SHADER_TEXTARGET_MTRL_AMBIENT)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_SPECULAR, m_pShader, SHADER_TEXTARGET_MTRL_SPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_SPECULAR");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MATERIAL_EMISSIVE, m_pShader, SHADER_TEXTARGET_MTRL_EMISSIVE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_EMISSIVE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_LIGHT_DIFFUSE, m_pShader, SHADER_TEXTARGET_LIGHTDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_LIGHT_AMBIENT, m_pShader, SHADER_TEXTARGET_LIGHTDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_AMBIENT");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_LIGHT_SPECULAR, m_pShader, SHADER_TEXTARGET_LIGHTSPECULAR)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Bind_RenderTarget: RENDERTARGET_LIGHT_SPECULAR");
	}

	if (FAILED(m_pShader->BeginPass(2)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Render");
	}

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::NONLIGHT)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::NONLIGHT)].clear();

	return hr;
}

HRESULT CRenderer::Render_Blend()
{
	HRESULT hr = S_OK;

	_float3 vPosition = _float3(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::POSITION));

	m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)].sort(
		[vPosition](shared_ptr<ISystem> _pSystemA, shared_ptr<ISystem> _pSystemB)->_bool
		{
			_float3 vPositionA = static_pointer_cast<CTransform>(static_pointer_cast<CGameObject>(_pSystemA)->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);
			_float3 vPositionB = static_pointer_cast<CTransform>(static_pointer_cast<CGameObject>(_pSystemB)->Get_Component(COMPONENT::TRANSFORM))->Get_State(TRANSFORM::POSITION);

			return (vPosition - vPositionA).length() > (vPosition - vPositionB).length();
		}
	);

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::BLEND)].clear();

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Blend", "Failed to End_MultiRenderTarget");
	}

	return hr;
}

HRESULT CRenderer::Draw_PostProcess()
{
	if (FAILED(m_pRenderTarget_Manager->Begin_MultiRenderTaget(MULTIRENDERTARGET_POSTPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PreProcess", "Failed to Begin_MultiRenderTaget: MULTIRENDERTARGET_PREPROCESS");
	}

	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_PREPROCESS, m_pShader, SHADER_TEXTARGET_PREPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_DIFFUSE");
	}
	if (FAILED(m_pRenderTarget_Manager->Bind_RenderTarget(RENDERTARGET_MASK, m_pShader, SHADER_TEXTARGET_MASK)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Bind_RenderTarget: RENDERTARGET_MATERIAL_AMBIENT");
	}

	if (FAILED(m_pShader->BeginPass(3)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to BeginPass");
	}
	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Draw_PostProcess", "Failed to Render");
	}

	if (FAILED(m_pRenderTarget_Manager->End_MultiRenderTarget(RENDERTARGET_POSTPROCESS)))
	{
		MSG_RETURN(E_FAIL, "CRenderer::Render_Blend", "Failed to End_MultiRenderTarget");
	}

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::UI)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::UI)].clear();

	return hr;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	HRESULT hr = S_OK;

	for (auto& pSystem : m_lstRenderGroup[IDX(RENDER_GROUP::DEBUG)])
	{
		if (nullptr != pSystem)
		{
			if (FAILED(pSystem->Render()))
			{
				hr = E_FAIL;
			}
		}
	}
	m_lstRenderGroup[IDX(RENDER_GROUP::DEBUG)].clear();

	return hr;
}
#endif
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
