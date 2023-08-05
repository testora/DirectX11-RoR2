#include "ClientPCH.h"
#include "Background.h"
#include "GameInstance.h"

CBackground::CBackground(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CBackground::CBackground(const CBackground& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CBackground::Initialize(any _arg)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::TEXTURE) | BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, any());
	m_umapComponentArg[COMPONENT::TEXTURE]			= make_pair(PROTOTYPE_COMPONENT_TEXTURE_BACKGROUND, any());
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, any());

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Failed to __super::Initialize");
	}

	m_pTransformCom->Set_Scale(_float3(g_iWinCX, g_iWinCY, 1.f));
	m_pTransformCom->Set_State(CTransform::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	CPipeLine::Get_Instance()->Set_Transform(CPipeLine::VIEW, XMMatrixIdentity());
	CPipeLine::Get_Instance()->Set_Transform(CPipeLine::PROJ, XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f));

	return S_OK;
}

void CBackground::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CBackground::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CBackground::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to __super::Render");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix("g_mWorld", m_pTransformCom->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix("g_mView", CPipeLine::Get_Instance()->Get_Transform(CPipeLine::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix("g_mProj", CPipeLine::Get_Instance()->Get_Transform(CPipeLine::PROJ))))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pTextureCom->Bind_ShaderResourceView(m_pShaderCom, "g_texDiffuse", 0)))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to Bind_ShaderResourceView");
	}

	if (FAILED(m_pShaderCom->BeginPass(0)))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to BeginPass");
	}

	if (FAILED(m_pVIBufferCom->Render()))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to Render");
	}

	return S_OK;
}

HRESULT CBackground::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBackground::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransformCom = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransformCom)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pTransformCom");
	}

	m_pRendererCom = dynamic_pointer_cast<CRenderer>(m_umapComponent[COMPONENT::RENDERER]);
	if (nullptr == m_pRendererCom)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pRendererCom");
	}

	m_pShaderCom = dynamic_pointer_cast<CShader>(m_umapComponent[COMPONENT::SHADER]);
	if (nullptr == m_pShaderCom)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pShaderCom");
	}

	m_pTextureCom = dynamic_pointer_cast<CTexture>(m_umapComponent[COMPONENT::TEXTURE]);
	if (nullptr == m_pTextureCom)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pTextureCom");
	}

	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Rect>(m_umapComponent[COMPONENT::VIBUFFER_RECT]);
	if (nullptr == m_pVIBufferCom)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pVIBufferCom");
	}

	return S_OK;
}

shared_ptr<CBackground> CBackground::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CBackground> pInstance = make_private_shared(CBackground, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CBackground::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBackground::Clone(any _arg)
{
	shared_ptr<CBackground> pInstance = make_private_shared_copy(CBackground, *this);

	if (FAILED(pInstance->Initialize(_arg)))
	{
		MSG_RETURN(nullptr, "CBackground::Clone", "Failed to Initialize");
	}

	return pInstance;
}
