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

	m_umapBehaviorArg[BEHAVIOR::GROUNDING]	= make_pair(wstring(), wstring(GRID_TERRAIN));

	m_tCharacterDesc.fForwardSpeed			= 100.f;
	m_tCharacterDesc.fBackwardSpeed			= 100.f;
	m_tCharacterDesc.fLeftSpeed				= 100.f;
	m_tCharacterDesc.fRightSpeed			= 100.f;
	m_tCharacterDesc.fJumpPower				= 30.f;

	m_tCharacterDesc.vMaxSpeed				= _float3(200.f, 200.f, 200.f);
	m_tCharacterDesc.vResist				= _float3(0.005f, 0.1f, 0.005f);

	return S_OK;
}

HRESULT CRailGunner::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Initialize", "Failed to __super::Initialize");
	}

	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, _float4(1.f, 5.f, 1.f, 1.f));

	return S_OK;
}

void CRailGunner::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CRailGunner::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

#ifdef _DEBUG
#if ACTIVATE_IMGUI
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		ImGui::Begin("RailGunner");

		ImGui::Text("Position: ");
		ImGui::Text("X: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).x);
		ImGui::SameLine();
		ImGui::Text("Y: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).y);
		ImGui::SameLine();
		ImGui::Text("Z: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).z);

		auto pVelocity = Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS)->Get_Velocity();

		ImGui::Text("Physics: ");
		ImGui::DragFloat3("Velocity", reinterpret_cast<_float*>(&pVelocity));

		ImGui::Text("Desc: ");
		ImGui::SliderFloat("ForwardSpeed", &m_tCharacterDesc.fForwardSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("BackwardSpeed", &m_tCharacterDesc.fBackwardSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("LeftSpeed", &m_tCharacterDesc.fLeftSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("RightSpeed", &m_tCharacterDesc.fRightSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("JumpPower", &m_tCharacterDesc.fJumpPower, 0.f, 1000.f);
		ImGui::SliderFloat3("MaxSpeed", reinterpret_cast<_float*>(&m_tCharacterDesc.vMaxSpeed), 0.f, 1000.f);
		ImGui::SliderFloat3("Resist", reinterpret_cast<_float*>(&m_tCharacterDesc.vResist), 0.f, 0.1f);

		ImGui::End();
	}
#endif
#endif

	m_pRenderer->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CRailGunner::Render(_uint)
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

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pRenderer");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	return S_OK;
}

shared_ptr<CRailGunner> CRailGunner::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRailGunner> pInstance = make_private_shared(CRailGunner, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CRailGunner::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRailGunner::Clone(any)
{
	shared_ptr<CRailGunner> pInstance = make_private_shared_copy(CRailGunner, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRailGunner::Create", "Failed to Initialize");
	}

	return pInstance;
}
