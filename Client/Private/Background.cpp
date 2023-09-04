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

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE]			= make_pair(PROTOTYPE_COMPONENT_TEXTURE_BACKGROUND, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform->Set_Scale(_float3(g_iWinCX, g_iWinCY, 1.f));
	m_pTransform->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	auto a = XMVector3TransformCoord(_float4(50.f, 100.f, 0.f, 1.f), (m_pTransform->Get_Matrix() * XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));

	return S_OK;
}

void CBackground::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CBackground::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRenderer->Add_RenderObject(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CBackground::Render()
{
	if (FAILED(m_pTexture->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to Bind_ShaderResourceViews");
	}

	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CBackground::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CBackground::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBackground::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Nullptr Exception: m_pTransform");
	}

	m_pRenderer = dynamic_pointer_cast<CRenderer>(m_umapComponent[COMPONENT::RENDERER]);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Nullptr Exception: m_pRenderer");
	}

	m_pShader = dynamic_pointer_cast<CShader>(m_umapComponent[COMPONENT::SHADER]);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Nullptr Exception: m_pShader");
	}

	m_pTexture = dynamic_pointer_cast<CTexture>(m_umapComponent[COMPONENT::TEXTURE]);
	if (nullptr == m_pTexture)
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Nullptr Exception: m_pTexture");
	}

	m_pVIBuffer = dynamic_pointer_cast<CVIBuffer_Rect>(m_umapComponent[COMPONENT::VIBUFFER_RECT]);
	if (nullptr == m_pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Nullptr Exception: m_pVIBuffer");
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
