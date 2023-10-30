#include "ClientPCH.h"
#include "Brother.h"
#include "GameInstance.h"
#include "Brother_BehaviorTree.h"
#include "BrotherUI_HealthBar.h"
#include "VFX_TrailLine.h"
#include "VFX_TrailQuad.h"
#include "RailGunner.h"

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
	tColliderDesc.eGroup	= COLLISION_GROUP::MONSTER;
	tColliderDesc.vPosition = _float3(0.f, .5f, 0.f);
	tColliderDesc.vSize		= _float3(1.f, 1.f, 1.f);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM, g_aNull);
	m_umapComponentArg[COMPONENT::COLLIDER]	= make_pair(PROTOTYPE_COMPONENT_COLLIDER, tColliderDesc);
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_BROTHER, g_aNull);

	m_umapBehaviorArg[BEHAVIOR::GROUNDING]	= make_pair(wstring(), wstring(GRID_TERRAIN));

	m_tEntityDesc.fMaxHP					= 50.f;
	m_tEntityDesc.fHP						= m_tEntityDesc.fMaxHP;

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

	LIGHTDESC tLightDesc{};
	tLightDesc.eLightType	= LIGHTTYPE::SHADOW;
	tLightDesc.eShadowType	= SHADOWTYPE::DIRECTIONAL;
	tLightDesc.fRange		= 10.f;
	tLightDesc.vDirection	= _float3(-0.64f, -0.76f, -0.12f);
	CGameInstance::Get_Instance()->Add_Light(SCENE::MOON, tLightDesc, m_pTransform, shared_from_gameobject());

	m_pHealthBar = dynamic_pointer_cast<CBrotherUI_HealthBar>(CGameInstance::Get_Instance()->Clone_GameObject(
		CGameInstance::Get_Instance()->Current_Scene(), PROTOTYPE_GAMEOBJECT_BROTHER_HEALTHBAR, &m_tEntityDesc));
		
	return S_OK;
}

void CBrother::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

//	if (m_pHealthBar)
	{
		m_pHealthBar->Tick(_fTimeDelta);
	}

//	if (CGameInstance::Get_Instance()->Key_Down('Y'))
//	{
//		m_pTransform->Rotate_Lerp(TRANSFORM::UP, XMConvertToRadians(90.f), 10.f);
//	}

	if (CGameInstance::Get_Instance()->Key_Down(VK_MBUTTON))
	{
		m_tEntityDesc.fHP = 50.f;
	}

#if ACTIVATE_IMGUI
	static _float fAnimationSpeed = 1.f;
	ImGui::Begin("Brother Animation");
	ImGui::DragFloat("Animation Speed", &fAnimationSpeed, 0.01f, 0.f, 2.f);
	if (ImGui::Button("IDLE_READY"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::IDLE_READY, fAnimationSpeed);
	if (ImGui::Button("SPELL_CHANNEL_ENTER"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER, fAnimationSpeed);
	if (ImGui::Button("SPELL_CHANNEL_LOOP"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_LOOP, fAnimationSpeed);
	if (ImGui::Button("SPELL_CHANNEL_TO_IDLE"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE, fAnimationSpeed);
	if (ImGui::Button("JUMP_FORWARD"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::JUMP_FORWARD, fAnimationSpeed);
	if (ImGui::Button("AIR_LOOP_UP"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_UP, fAnimationSpeed);
	if (ImGui::Button("AIR_LOOP_DOWN"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_DOWN, fAnimationSpeed);
	if (ImGui::Button("AIR_LOOP_DOWN_FORWARD"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_DOWN_FORWARD, fAnimationSpeed);
	if (ImGui::Button("AIR_LOOP_DOWN_BACKWARD"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIR_LOOP_DOWN_BACKWARD, fAnimationSpeed);
	if (ImGui::Button("AIM_YAW"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIM_YAW, fAnimationSpeed);
	if (ImGui::Button("AIM_PITCH"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::AIM_PITCH, fAnimationSpeed);
	if (ImGui::Button("RUN_FORWARD_READY"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::RUN_FORWARD_READY, fAnimationSpeed);
	if (ImGui::Button("RUN_TO_IDLE"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::RUN_TO_IDLE, fAnimationSpeed);
	if (ImGui::Button("SPRINT_FORWARD"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPRINT_FORWARD, fAnimationSpeed);
	if (ImGui::Button("SPRINT_SMASH"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SPRINT_SMASH, fAnimationSpeed);
	if (ImGui::Button("DASH_FORWARD"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_FORWARD, fAnimationSpeed);
	if (ImGui::Button("DASH_BACKWARD"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_BACKWARD, fAnimationSpeed);
	if (ImGui::Button("DASH_LEFT"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_LEFT, fAnimationSpeed);
	if (ImGui::Button("DASH_RIGHT"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DASH_RIGHT, fAnimationSpeed);
	if (ImGui::Button("SMASH_FORWARD"))					{Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::SMASH_FORWARD, fAnimationSpeed);CGameInstance::Get_Instance()->Play_Sound(TEXT("brother_smash"), SOUND_CHANNEL::MONSTER_WEAPON); }
	if (ImGui::Button("LUNARSHARD_FIRE_FORWARD"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LUNARSHARD_FIRE_FORWARD, fAnimationSpeed);
	if (ImGui::Button("ULT_ENTER"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::ULT_ENTER, fAnimationSpeed);
	if (ImGui::Button("ULT_CHANNEL"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::ULT_CHANNEL, fAnimationSpeed);
	if (ImGui::Button("ULT_EXIT"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::ULT_EXIT, fAnimationSpeed);
	if (ImGui::Button("FLINCH1"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::FLINCH1, fAnimationSpeed);
	if (ImGui::Button("FLINCH2"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::FLINCH2, fAnimationSpeed);
	if (ImGui::Button("FLINCH3"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::FLINCH3, fAnimationSpeed);
	if (ImGui::Button("THRONE"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::THRONE, fAnimationSpeed);
	if (ImGui::Button("THRONE_TO_IDLE"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::THRONE_TO_IDLE, fAnimationSpeed);
	if (ImGui::Button("LEAP_BEGIN"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LEAP_BEGIN, fAnimationSpeed);
	if (ImGui::Button("LEAP_END"))						Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LEAP_END, fAnimationSpeed);
	if (ImGui::Button("LIGHTIMPACT"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::LIGHTIMPACT, fAnimationSpeed);
	if (ImGui::Button("TPOSE"))							Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::TPOSE, fAnimationSpeed);
	if (ImGui::Button("HURT_IDLE_LOOP"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_LOOP, fAnimationSpeed);
	if (ImGui::Button("HURT_IDLE_SINGLE"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_IDLE_SINGLE, fAnimationSpeed);
	if (ImGui::Button("HURT_FISTSLAM"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_FISTSLAM, fAnimationSpeed);
	if (ImGui::Button("HURT_WALK_FORWARD"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_WALK_FORWARD, fAnimationSpeed);
	if (ImGui::Button("HURT_LUNARSHARD_EXIT_LEFT"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_LEFT, fAnimationSpeed);
	if (ImGui::Button("HURT_LUNARSHARD_EXIT_RIGHT"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_RIGHT, fAnimationSpeed);
	if (ImGui::Button("HURT_LUNARSHARD_FIRE_FORWARD"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_FORWARD, fAnimationSpeed);
	if (ImGui::Button("HURT_LUNARSHARD_FIRE_LEFT"))		Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_LEFT, fAnimationSpeed);
	if (ImGui::Button("HURT_LUNARSHARD_FIRE_RIGHT"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_FIRE_RIGHT, fAnimationSpeed);
	if (ImGui::Button("HURT_LUNARSHARD_EXIT_FORWARD"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_EXIT_FORWARD, fAnimationSpeed);
	if (ImGui::Button("HURT_LUNARSHARD_ENTER_FORWARD"))	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_LUNARSHARD_ENTER_FORWARD, fAnimationSpeed);
	if (ImGui::Button("HURT_STAGGER_ENTER"))			Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_STAGGER_ENTER, fAnimationSpeed);
	if (ImGui::Button("HURT_STRGGER_EXIT"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_STRGGER_EXIT, fAnimationSpeed);
	if (ImGui::Button("HURT_STAGGER_LOOP"))				Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_STAGGER_LOOP, fAnimationSpeed);
	if (ImGui::Button("HURT_TO_DEATH"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::HURT_TO_DEATH, fAnimationSpeed);
	if (ImGui::Button("DEATH_LOOP"))					Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::BROTHER::DEATH_LOOP, fAnimationSpeed);
	ImGui::End();
#endif

	if (!m_bRender)
	{
		m_pTransform->Set_State(TRANSFORM::POSITION, _float3(0.f, 0.f, 0.f));
	}
}

void CBrother::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

//	if (m_pHealthBar)
	{
		m_pHealthBar->Late_Tick(_fTimeDelta);
	}

	if (m_bRender)
	{
		Add_RenderObject(RENDER_GROUP::SHADOW);
		Add_RenderObject(RENDER_GROUP::NONBLEND);
	}
}

HRESULT CBrother::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CBrother::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CBrother::Render_ShadowDepth()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CBrother::Render_ShadowDepth", "Failed to Bind_OnShader");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CBrother::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CBrother::Fetch(any _vPosition3)
{
	if (FAILED(__super::Fetch(_vPosition3)))
	{
		MSG_RETURN(E_FAIL, "CBrother::Fetch", "Failed to __super::Fetch");
	}

	m_pVFX_EyeTrail = dynamic_pointer_cast<CVFX_TrailLine>(
		CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), POOL_EFFECT_TRAIL_LINE)->Pop(
		make_pair(shared_from_gameobject(), "eyeball")));

	m_pVFX_EyeTrail->Set_Color(_color(0.2f, 0.6f, 0.8f, 1.f));
	m_pVFX_EyeTrail->Set_Thickness(0.05f);

	m_pVFX_HammerTrail = dynamic_pointer_cast<CVFX_TrailQuad>(
		CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), POOL_EFFECT_TRAIL_QUAD)->Pop(
		make_pair(shared_from_gameobject(), make_pair("2HWeaponBendy.5", "2HWeaponBendy.7"))));

	m_pVFX_HammerTrail->Set_Interval(0.001f);
	m_pVFX_HammerTrail->Set_RelativeLength(2.f, 4.f);
	m_pVFX_HammerTrail->Set_Texture(PROTOTYPE_COMPONENT_TEXTURE_EFFECT_BROTHER_HAMMERTRAIL);

	return S_OK;
}

_bool CBrother::Return()
{
	return false;
}

void CBrother::OnCollisionEnter(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pOther, _float _fTimeDelta)
{
	if (COLLISION_GROUP::PLAYER_BULLET == _eGroup)
	{
		m_tEntityDesc.fHP -= 1.f;
	}
}

void CBrother::OnCollision(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pOther, _float _fTimeDelta)
{
	int a = 1;
}

void CBrother::OnCollisionExit(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pOther, _float _fTimeDelta)
{
	int a = 1;
}

HRESULT CBrother::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBrother::Ready_Components", "Failed to __super::Ready_Components");
	}

	_uint iHammerIndex			= m_pModel->Get_MeshIndex("BrotherHammerConcrete");
	_uint iConstellationIndex	= m_pModel->Get_MeshIndex("mdlConstellationBrother");

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER),		iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_ENTER),		iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_LOOP),		iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_LOOP),		iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE),	iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::SPELL_CHANNEL_TO_IDLE),	iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_LOOP),			iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_LOOP),			iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_SINGLE),		iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_IDLE_SINGLE),		iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_FISTSLAM),			iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_FISTSLAM),			iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_WALK_FORWARD),		iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_WALK_FORWARD),		iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STAGGER_ENTER),		iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STAGGER_ENTER),		iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRGGER_EXIT),		iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STRGGER_EXIT),		iConstellationIndex);

	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STAGGER_LOOP),		iHammerIndex);
	m_pModel->Hide_MeshFromAnimations(IDX(ANIMATION::BROTHER::HURT_STAGGER_LOOP),		iConstellationIndex);

	m_pModel->Set_DefaultAnimation(IDX(ANIMATION::BROTHER::IDLE_READY));
	
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

void CBrother::Hit()
{
	m_tEntityDesc.fHP -= 1.f;

	if (m_pAnimator->Is_Playing(ANIMATION::BROTHER::SPRINT_FORWARD))
	{
		switch (rand() % 3)
		{
		case 0:
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::FLINCH1, 1.f, false, g_fDefaultInterpolationDuration, false);
			break;
		case 1:
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::FLINCH2, 1.f, false, g_fDefaultInterpolationDuration, false);
			break;
		case 2:
			m_pAnimator->Play_Animation(ANIMATION::BROTHER::FLINCH3, 1.f, false, g_fDefaultInterpolationDuration, false);
			break;
		}
	}
}

void CBrother::Hit_WeakPoint()
{
	m_tEntityDesc.fHP -= 5.f;
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
