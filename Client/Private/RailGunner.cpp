#include "ClientPCH.h"
#include "RailGunner.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"

CRailGunner::CRailGunner(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CRailGunner::CRailGunner(const CRailGunner& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CRailGunner::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM) | BIT(COMPONENT::SHADER) | BIT(COMPONENT::MODEL);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS) | BIT(BEHAVIOR::CONTROL) | BIT(BEHAVIOR::GROUNDING);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM, any());
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_RAILGUNNER, any());

	m_tCharacterDesc.fForwardSpeed			= 10.f;
	m_tCharacterDesc.fBackwardSpeed			= 10.f;
	m_tCharacterDesc.fLeftSpeed				= 10.f;
	m_tCharacterDesc.fRightSpeed			= 10.f;

	return S_OK;
}

HRESULT CRailGunner::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Initialize", "Failed to __super::Initialize");
	}

	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, _float4(1.f, 5.f, 1.f, 1.f));
	Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS)->Set_MaxSpeed(_float3(0.1f, 0.1f, 0.1f));
	Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS)->Set_Resist(_float3(0.01f, 0.01f, 0.01f));

	return S_OK;
}

void CRailGunner::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_pModelCom->Play_Animation(_fTimeDelta);
}

void CRailGunner::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

#ifdef _DEBUG
	if (ACTIVATE_IMGUI)
	{
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			ImGui::Begin("RailGunner");
			ImGui::Text("Position: ");
			ImGui::Text("X: %f\t", m_pTransformCom->Get_State(TRANSFORM::POSITION).x);
			ImGui::SameLine();
			ImGui::Text("Y: %f\t", m_pTransformCom->Get_State(TRANSFORM::POSITION).y);
			ImGui::SameLine();
			ImGui::Text("Z: %f\t", m_pTransformCom->Get_State(TRANSFORM::POSITION).z);
			ImGui::SameLine();
			ImGui::Text("W: %f\t", m_pTransformCom->Get_Matrix()._44);
			ImGui::End();
		}
	}
#endif

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

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_VIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_Matrix");
	}

	if (FAILED(m_pShaderCom->Bind_Matrix(SHADER_MATRIX_PROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJ))))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_Matrix");
	}

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshes(); ++i)
	{
		if (FAILED(m_pModelCom->Bind_ShaderResourceViews(i, m_pShaderCom, aiTextureType_DIFFUSE, "g_texDiffuse")))
		{
			MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_ShaderResourceViews");
		}

		if (FAILED(m_pModelCom->Bind_BoneMatrices(i, m_pShaderCom, "g_mBones")))
		{
			MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Bind_BoneMatrices");
		}

		if (FAILED(m_pShaderCom->BeginPass(0)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to BeginPass");
		}

		if (FAILED(m_pModelCom->Render(i)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to Render");
		}
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

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTerrain::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRailGunner::Clone(any _arg)
{
	shared_ptr<CRailGunner> pInstance = make_private_shared_copy(CRailGunner, *this);

	if (FAILED(pInstance->Initialize(_arg)))
	{
		MSG_RETURN(nullptr, "CTerrain::Create", "Failed to Initialize");
	}

	return pInstance;
}
