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
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS)	| BIT(BEHAVIOR::CONTROL)	| BIT(BEHAVIOR::GROUNDING);

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

	m_pModel->Play_Animation(_fTimeDelta);
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
			ImGui::Text("X: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).x);
			ImGui::SameLine();
			ImGui::Text("Y: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).y);
			ImGui::SameLine();
			ImGui::Text("Z: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).z);
			ImGui::SameLine();
			ImGui::Text("W: %f\t", m_pTransform->Get_Matrix()._44);
			ImGui::End();
		}
	}
#endif

	m_pRenderer->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CRailGunner::Render(_uint _iPassIndex)
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CRailGunner::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pRenderer = dynamic_pointer_cast<CRenderer>(m_umapComponent[COMPONENT::RENDERER]);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pRenderer");
	}

	m_pShader = dynamic_pointer_cast<CShader>(m_umapComponent[COMPONENT::SHADER]);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pShader");
	}

	m_pModel = dynamic_pointer_cast<CModel>(m_umapComponent[COMPONENT::MODEL]);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pModel");
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
