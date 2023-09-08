#include "ClientPCH.h"
#include "RailGunner.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Control_RailGunner.h"
#include "RailGunner_Crosshair.h"

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
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::COLLIDER)	| BIT(COMPONENT::MODEL);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS)	| BIT(BEHAVIOR::GROUNDING)	| BIT(BEHAVIOR::ANIMATOR)	| BIT(BEHAVIOR::CONTROL);

	COLLIDERDESC tColliderDesc{};
	tColliderDesc.eType		= COLLIDER::AABB;
	tColliderDesc.vPosition = _float3(0.f, .5f, 0.f);
	tColliderDesc.vSize		= _float3(1.f, 1.f, 1.f);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM, g_aNull);
	m_umapComponentArg[COMPONENT::COLLIDER]	= make_pair(PROTOTYPE_COMPONENT_COLLIDER, tColliderDesc);
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_RAILGUNNER, g_aNull);

	m_umapBehaviorArg[BEHAVIOR::GROUNDING]	= make_pair(wstring(), wstring(GRID_TERRAIN));

	m_tCharacterDesc.fForwardSpeed			= PLAYER_SPEED_FORWARD;
	m_tCharacterDesc.fBackwardSpeed			= PLAYER_SPEED_BACKWARD;
	m_tCharacterDesc.fLeftSpeed				= PLAYER_SPEED_LEFT;
	m_tCharacterDesc.fRightSpeed			= PLAYER_SPEED_RIGHT;
	m_tCharacterDesc.fJumpPower				= PLAYER_JUMP_POWER;

	m_tCharacterDesc.vMaxSpeed				= PLAYER_SPEED_TERMINAL;
	m_tCharacterDesc.vResist				= PLAYER_SPEED_RESIST;

	return S_OK;
}

HRESULT CRailGunner::Initialize(any)
{
	m_pCrosshair = dynamic_pointer_cast<CRailGunner_Crosshair>(CGameInstance::Get_Instance()->Clone_GameObject(
		CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_RAILGUNNER_CROSSHAIR));

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform->Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	return S_OK;
}

void CRailGunner::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_pCrosshair->Tick(_fTimeDelta);
}

void CRailGunner::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

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
		ImGui::SliderFloat("ForwardSpeed",	&m_tCharacterDesc.fForwardSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("BackwardSpeed",	&m_tCharacterDesc.fBackwardSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("LeftSpeed",		&m_tCharacterDesc.fLeftSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("RightSpeed",	&m_tCharacterDesc.fRightSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("JumpPower",		&m_tCharacterDesc.fJumpPower, 0.f, 1000.f);
		ImGui::SliderFloat3("MaxSpeed",		reinterpret_cast<_float*>(&m_tCharacterDesc.vMaxSpeed), 0.f, 1000.f);
		ImGui::SliderFloat3("Resist",		reinterpret_cast<_float*>(&m_tCharacterDesc.vResist), 0.f, 0.5f);

		ImGui::End();
	}
#endif

	m_pRenderer->Add_RenderObject(RENDER_GROUP::PRIORITY, shared_from_this());

	m_pCrosshair->Late_Tick(_fTimeDelta);
}

HRESULT CRailGunner::Render()
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

HRESULT CRailGunner::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_umapBehavior.emplace(BEHAVIOR::CONTROL, CControl_RailGunner::Create(shared_from_this(), m_pCrosshair, &m_tCharacterDesc));

	return S_OK;
}

void CRailGunner::Set_State(STATE _eState, const _bool _bState)
{
	m_bitPlayerState.set(IDX(_eState), _bState);
}

_bool CRailGunner::Check_State(STATE _eState) const
{
	return m_bitPlayerState.test(IDX(_eState));
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
