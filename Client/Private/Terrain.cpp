#include "ClientPCH.h"
#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTerrain::CTerrain(const CTerrain& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTerrain::Initialize(any _strHeightMapPath)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::TEXTURE) | BIT(COMPONENT::VIBUFFER_TERRAIN);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX, any());
	m_umapComponentArg[COMPONENT::TEXTURE]			= make_pair(PROTOTYPE_COMPONENT_TEXTURE_TERRAIN, any());
	m_umapComponentArg[COMPONENT::VIBUFFER_TERRAIN]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_TERRAIN, _strHeightMapPath);

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBackground::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CTerrain::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTerrain::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	if (CGameInstance::Get_Instance()->Key_Down(VK_MBUTTON))
	{
		_float3 vPos;
		if (m_pVIBufferCom->Intersect(vPos, m_pTransformCom->Get_Matrix()))
		{
			int test = 0;
		}
	}

	m_pRendererCom->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CTerrain::Render()
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

HRESULT CTerrain::Ready_Components()
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

	m_pVIBufferCom = dynamic_pointer_cast<CVIBuffer_Terrain>(m_umapComponent[COMPONENT::VIBUFFER_TERRAIN]);
	if (nullptr == m_pVIBufferCom)
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Nullptr Exception: m_pVIBufferCom");
	}

	return S_OK;
}

shared_ptr<CTerrain> CTerrain::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTerrain> pInstance = make_private_shared(CTerrain, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTerrain::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTerrain::Clone(any _strHeightMapPath)
{
	shared_ptr<CTerrain> pInstance = make_private_shared_copy(CTerrain, *this);

	if (FAILED(pInstance->Initialize(_strHeightMapPath)))
	{
		MSG_RETURN(nullptr, "CTerrain::Clone", "Failed to Initialize");
	}

	return pInstance;
}
