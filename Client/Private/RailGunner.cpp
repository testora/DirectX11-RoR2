#include "ClientPCH.h"
#include "RailGunner.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "Control_RailGunner.h"
#include "System.h"
#include "RailGunner_State.h"
#include "RailGunner_Crosshair.h"
#include "RailGunner_PistolBullet.h"

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
	m_tCharacterDesc.fSpritPower			= PLAYER_SPRINT_POWER;
	m_tCharacterDesc.fJumpPower				= PLAYER_JUMP_POWER;

	m_tCharacterDesc.vMaxSpeed				= PLAYER_SPEED_TERMINAL;
	m_tCharacterDesc.vResist				= PLAYER_SPEED_RESIST;

	return S_OK;
}

HRESULT CRailGunner::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Initialize", "Failed to __super::Initialize");
	}
	
	if (FAILED(Ready_RailGunner()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Initialize", "Failed to Ready_RailGunner");
	}
	
	if (FAILED(Ready_Bullets()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Initialize", "Failed to Ready_Bullets");
	}
	
	m_pTransform->Set_Scale(_float3(1.2f, 1.2f, 1.2f));

	return S_OK;
}

void CRailGunner::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	for (auto& system : m_umapSystem)
	{
		system.second->Tick(_fTimeDelta);
	}

	if (CGameInstance::Get_Instance()->Key_Down('J'))
	{
		CGameInstance::Get_Instance()->Find_Pool(SCENE::TEST, SCENE_TEST_POOL_RAILGUNNER_PISTOLBULLET)->Pop(m_pTransform->Get_State(TRANSFORM::POSITION));
	}
}

void CRailGunner::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	for (auto& system : m_umapSystem)
	{
		system.second->Late_Tick(_fTimeDelta);
	}

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

	Add_RenderObject(RENDER_GROUP::PRIORITY);
}

HRESULT CRailGunner::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner::Render", "Failed to __super::Render");
	}

	for (auto& system : m_umapSystem)
	{
		system.second->Render();
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

	m_umapBehavior.emplace(BEHAVIOR::CONTROL, CControl_RailGunner::Create(shared_from_this(), &m_tCharacterDesc));

	return S_OK;
}

HRESULT CRailGunner::Ready_RailGunner()
{
	m_umapSystem.reserve(IDX(SYSTEM::MAX));

	m_umapSystem[SYSTEM::STATE] = CRailGunner_State::Create(dynamic_pointer_cast<CRailGunner>(shared_from_this()));
	m_umapSystem[SYSTEM::CROSSHAIR] = CRailGunner_Crosshair::Create(m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CRailGunner::Ready_Bullets()
{
	CGameInstance::Get_Instance()->Add_Pool(SCENE::TEST, SCENE_TEST_POOL_RAILGUNNER_PISTOLBULLET, PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET, 50);

	return S_OK;
}

#pragma region RailGunner State

_bool CRailGunner::Is_State(bitset<IDX(RG_STATE::MAX)> _bit)
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::STATE)
	||	nullptr == m_umapSystem.find(SYSTEM::STATE)->second)
	{
		MSG_RETURN(false, "CRailGunner::Is_State", "Nullptr Exception: m_pState");
	}

	return static_pointer_cast<CRailGunner_State>(m_umapSystem[SYSTEM::STATE])->Is_State(_bit);
}

bitset<IDX(RG_STATE::MAX)> CRailGunner::Get_State()
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::STATE)
	||	nullptr == m_umapSystem.find(SYSTEM::STATE)->second)
	{
		MSG_RETURN(bitset<IDX(RG_STATE::MAX)>(0), "CRailGunner::Get_State", "Nullptr Exception: m_pState");
	}

	return static_pointer_cast<CRailGunner_State>(m_umapSystem[SYSTEM::STATE])->Get_State();
}

void CRailGunner::Set_State(RG_STATE _eState, _bool _bValue)
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::STATE)
	||	nullptr == m_umapSystem.find(SYSTEM::STATE)->second)
	{
		MSG_RETURN(, "CRailGunner::Set_State", "Nullptr Exception: m_pState");
	}

	return static_pointer_cast<CRailGunner_State>(m_umapSystem[SYSTEM::STATE])->Set_State(_eState, _bValue);
}

#pragma endregion
#pragma region RailGunner Crosshair

RG_CROSSHAIR CRailGunner::Get_Crosshair()
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::CROSSHAIR)
	||	nullptr == m_umapSystem.find(SYSTEM::CROSSHAIR)->second)
	{
		MSG_RETURN(RG_CROSSHAIR::MAIN, "CRailGunner::Get_Crosshair", "Nullptr Exception: m_pCrosshair");
	}

	return static_pointer_cast<CRailGunner_Crosshair>(m_umapSystem[SYSTEM::CROSSHAIR])->Get_State();
}

_bool CRailGunner::Is_Crosshair(RG_CROSSHAIR _eCrosshair)
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::CROSSHAIR)
	||	nullptr == m_umapSystem.find(SYSTEM::CROSSHAIR)->second)
	{
		MSG_RETURN(false, "CRailGunner::Is_Crosshair", "Nullptr Exception: m_pCrosshair");
	}

	return _eCrosshair == static_pointer_cast<CRailGunner_Crosshair>(m_umapSystem[SYSTEM::CROSSHAIR])->Get_State();
}

_bool CRailGunner::Is_SuccessReload()
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::CROSSHAIR)
	||	nullptr == m_umapSystem.find(SYSTEM::CROSSHAIR)->second)
	{
		MSG_RETURN(false, "CRailGunner::Is_SuccessReload", "Nullptr Exception: m_pCrosshair");
	}

	return static_pointer_cast<CRailGunner_Crosshair>(m_umapSystem[SYSTEM::CROSSHAIR])->Is_SuccessReload();
}

void CRailGunner::Visualize_Crosshair(RG_CROSSHAIR _eCrosshair)
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::CROSSHAIR)
	||	nullptr == m_umapSystem.find(SYSTEM::CROSSHAIR)->second)
	{
		MSG_RETURN(, "CRailGunner::Visualize_Crosshair", "Nullptr Exception: m_pCrosshair");
	}

	return static_pointer_cast<CRailGunner_Crosshair>(m_umapSystem[SYSTEM::CROSSHAIR])->Visualize(_eCrosshair);
}

void CRailGunner::Bounce_Bracket()
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::CROSSHAIR)
	||	nullptr == m_umapSystem.find(SYSTEM::CROSSHAIR)->second)
	{
		MSG_RETURN(, "CRailGunner::Bounce_Bracket", "Nullptr Exception: m_pCrosshair");
	}

	return static_pointer_cast<CRailGunner_Crosshair>(m_umapSystem[SYSTEM::CROSSHAIR])->Bounce_Bracket();
}

void CRailGunner::Hit_Reload()
{
	if (m_umapSystem.end() == m_umapSystem.find(SYSTEM::CROSSHAIR)
	||	nullptr == m_umapSystem.find(SYSTEM::CROSSHAIR)->second)
	{
		MSG_RETURN(, "CRailGunner::Hit_Reload", "Nullptr Exception: m_pCrosshair");
	}

	return static_pointer_cast<CRailGunner_Crosshair>(m_umapSystem[SYSTEM::CROSSHAIR])->Hit_Reload();
}

#pragma endregion

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
