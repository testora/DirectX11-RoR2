#include "ClientPCH.h"
#include "RailGunner.h"
#include "GameInstance.h"

CRailGunner::CRailGunner(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CRailGunner::CRailGunner(const CRailGunner& _rhs)
	: CGameObject		(_rhs)
	, m_pRendererCom	(_rhs.m_pRendererCom)
	, m_pTransformCom	(_rhs.m_pTransformCom)
	, m_pShaderCom		(_rhs.m_pShaderCom)
	, m_pModelCom		(_rhs.m_pModelCom)
{
}

HRESULT CRailGunner::Initialize(any)
{
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESH, any());
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_RAILGUNNER, any());

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CRailGunner::Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

void CRailGunner::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CRailGunner::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to __super::Render");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_WORLD, m_pTransformCom->Get_Matrix())))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_VIEW, CPipeLine::Get_Instance()->Get_Transform(CPipeLine::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_PROJ, CPipeLine::Get_Instance()->Get_Transform(CPipeLine::PROJ))))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pModelCom->Bind_ShaderResourceView(m_pShaderCom, 0, aiTextureType_DIFFUSE, SHADER_TEXTURE_DIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_ShaderResourceView");
	}

	return S_OK;
}

HRESULT CRailGunner::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransformCom = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransformCom)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pTransformCom");
	}

	m_pRendererCom = dynamic_pointer_cast<CRenderer>(m_umapComponent[COMPONENT::RENDERER]);
	if (nullptr == m_pRendererCom)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pRendererCom");
	}

	m_pShaderCom = dynamic_pointer_cast<CShader>(m_umapComponent[COMPONENT::SHADER]);
	if (nullptr == m_pShaderCom)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pShaderCom");
	}

	m_pModelCom = dynamic_pointer_cast<CModel>(m_umapComponent[COMPONENT::MODEL]);
	if (nullptr == m_pModelCom)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pModelCom");
	}

	return S_OK;
}

shared_ptr<CRailGunner> CRailGunner::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRailGunner> pInstance = make_private_shared(CRailGunner, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTerrain::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRailGunner::Clone(any)
{
	return shared_from_this();
}
