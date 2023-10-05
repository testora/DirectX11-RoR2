#include "ClientPCH.h"
#include "Brother.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"

CBrother::CBrother(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CBrother::CBrother(const CBrother& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CBrother::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::COLLIDER)	| BIT(COMPONENT::MODEL);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS)	| BIT(BEHAVIOR::GROUNDING)	| BIT(BEHAVIOR::ANIMATOR)	| BIT(BEHAVIOR::CUSTOM);

	COLLIDERDESC tColliderDesc{};
	tColliderDesc.eType		= COLLIDER::AABB;
	tColliderDesc.vPosition = _float3(0.f, .5f, 0.f);
	tColliderDesc.vSize		= _float3(1.f, 1.f, 1.f);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM, g_aNull);
	m_umapComponentArg[COMPONENT::COLLIDER]	= make_pair(PROTOTYPE_COMPONENT_COLLIDER, tColliderDesc);
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_BROTHER, g_aNull);

	m_umapBehaviorArg[BEHAVIOR::GROUNDING]	= make_pair(wstring(), wstring(GRID_TERRAIN));

	m_tEntityDesc.fForwardSpeed				= BROTHER_SPEED_FORWARD;
	m_tEntityDesc.fBackwardSpeed			= BROTHER_SPEED_BACKWARD;
	m_tEntityDesc.fLeftSpeed				= BROTHER_SPEED_LEFT;
	m_tEntityDesc.fRightSpeed				= BROTHER_SPEED_RIGHT;
	m_tEntityDesc.fSpritPower				= BROTHER_SPRINT_POWER;
	m_tEntityDesc.fJumpPower				= BROTHER_JUMP_POWER;

	m_tEntityDesc.vMaxSpeed					= BROTHER_SPEED_TERMINAL;
	m_tEntityDesc.vResist					= BROTHER_SPEED_RESIST;

	return S_OK;
}

HRESULT CBrother::Initialize(any)
{
	if (FAILED(__super::Initialize("eyeball")))
	{
		MSG_RETURN(E_FAIL, "CBrother::Initialize", "Failed to __super::Initialize");
	}

	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::IDLE_READY);

	return S_OK;
}

void CBrother::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

#if ACTIVATE_IMGUI
	ImGui::Begin("Brother Animation");
	if (ImGui::Button("IDLE_READY"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::IDLE_READY);
	if (ImGui::Button("SPELL_CHANNEL_ENTER"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER);
	if (ImGui::Button("SPELL_CHANNEL_LOOP"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_LOOP);
	if (ImGui::Button("SPELL_CHANNEL_TO_IDLE"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE);
	if (ImGui::Button("JUMP_FORWARD"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::JUMP_FORWARD);
	if (ImGui::Button("AIR_LOOP_UP"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_UP);
	if (ImGui::Button("AIR_LOOP_DOWN"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_DOWN);
	if (ImGui::Button("AIR_LOOP_DOWN_FORWARD"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_DOWN_FORWARD);
	if (ImGui::Button("AIR_LOOP_DOWN_BACKWARD"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_DOWN_BACKWARD);
	if (ImGui::Button("AIM_YAW"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIM_YAW);
	if (ImGui::Button("AIM_PITCH"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIM_PITCH);
	if (ImGui::Button("RUN_FORWARD_READY"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::RUN_FORWARD_READY);
	if (ImGui::Button("RUN_TO_IDLE"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::RUN_TO_IDLE);
	if (ImGui::Button("SPRINT_FORWARD"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD);
	if (ImGui::Button("SPRINT_SMASH"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPRINT_SMASH);
	if (ImGui::Button("DASH_FORWARD"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_FORWARD);
	if (ImGui::Button("DASH_BACKWARD"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_BACKWARD);
	if (ImGui::Button("DASH_LEFT"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_LEFT);
	if (ImGui::Button("DASH_RIGHT"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_RIGHT);
	if (ImGui::Button("SMASH_FORWARD"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SMASH_FORWARD, 2.f);
	if (ImGui::Button("LUNARSHARD_FIRE_FORWARD"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LUNARSHARD_FIRE_FORWARD, 1.f, true, 0.1f, false);
	if (ImGui::Button("ULT_ENTER"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::ULT_ENTER);
	if (ImGui::Button("ULT_CHANNEL"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::ULT_CHANNEL);
	if (ImGui::Button("ULT_EXIT"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::ULT_EXIT);
	if (ImGui::Button("FLINCH1"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::FLINCH1);
	if (ImGui::Button("FLINCH2"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::FLINCH2);
	if (ImGui::Button("FLINCH3"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::FLINCH3);
	if (ImGui::Button("THRONE"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::THRONE);
	if (ImGui::Button("THRONE_TO_IDLE"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::THRONE_TO_IDLE);
	if (ImGui::Button("LEAP_BEGIN"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LEAP_BEGIN);
	if (ImGui::Button("LEAP_END"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LEAP_END);
	if (ImGui::Button("LIGHTIMPACT"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LIGHTIMPACT);
	if (ImGui::Button("TPOSE"))							Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::TPOSE);
	if (ImGui::Button("HURT_IDLE_LOOP"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_LOOP);
	if (ImGui::Button("HURT_IDLE_SINGLE"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_SINGLE);
	if (ImGui::Button("HURT_FISTSLAM"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_FISTSLAM);
	if (ImGui::Button("HURT_WALK_FORWARD"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_WALK_FORWARD);
	if (ImGui::Button("HURT_LUNARSHARD_EXIT_LEFT"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_LEFT);
	if (ImGui::Button("HURT_LUNARSHARD_EXIT_RIGHT"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_RIGHT);
	if (ImGui::Button("HURT_LUNARSHARD_FIRE_FORWARD"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_FORWARD);
	if (ImGui::Button("HURT_LUNARSHARD_FIRE_LEFT"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_LEFT);
	if (ImGui::Button("HURT_LUNARSHARD_FIRE_RIGHT"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_RIGHT);
	if (ImGui::Button("HURT_LUNARSHARD_EXIT_FORWARD"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_FORWARD);
	if (ImGui::Button("HURT_LUNARSHARD_ENTER_FORWARD"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_ENTER_FORWARD);
	if (ImGui::Button("HURT_STRAGGER_ENTER"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_STRAGGER_ENTER);
	if (ImGui::Button("HURT_STRAGGER_EXIT"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_STRAGGER_EXIT);
	if (ImGui::Button("HURT_STRAGGER_LOOP"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_STRAGGER_LOOP);
	if (ImGui::Button("HURT_TO_DEATH"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_TO_DEATH);
	if (ImGui::Button("DEATH_LOOP"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DEATH_LOOP);
	ImGui::End();
#endif
}

void CBrother::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CBrother::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CBrother::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CBrother::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Components", "Failed to __super::Ready_Components");
	}

	shared_ptr<CModel> pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == pModel)
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Components", "Failed to Get_Component: MODEL");
	}

	_uint iHammerIndex			= pModel->Get_MeshIndex("BrotherHammerConcrete");
	_uint iConstellationIndex	= pModel->Get_MeshIndex("mdlConstellationBrother");
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER),	iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER),	iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_LOOP),	iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_LOOP),	iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE),	iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE),	iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_LOOP),		iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_LOOP),		iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_SINGLE),		iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_SINGLE),		iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_FISTSLAM),			iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_FISTSLAM),			iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_WALK_FORWARD),		iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_WALK_FORWARD),		iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRAGGER_ENTER),	iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRAGGER_ENTER),	iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRAGGER_EXIT),	iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRAGGER_EXIT),	iConstellationIndex);

	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRAGGER_LOOP),	iHammerIndex);
	pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRAGGER_LOOP),	iConstellationIndex);

	pModel->Set_DefaultAnimation(IDX(ANIMATION::BROTHER::IDLE_READY));
	
	return S_OK;
}

HRESULT CBrother::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_umapBehavior.emplace(BEHAVIOR::CUSTOM, CBrother_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc));

	return S_OK;
}

void CBrother::Wander()
{
	return;
}

void CBrother::Hit()
{
}

void CBrother::Hit_WeakPoint()
{
}

shared_ptr<CBrother> CBrother::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CBrother> pInstance = make_private_shared(CBrother, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CBrother::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBrother::Clone(any)
{
	shared_ptr<CBrother> pInstance = make_private_shared_copy(CBrother, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CBrother::Clone", "Failed to Initialize");
	}

	return pInstance;
}
