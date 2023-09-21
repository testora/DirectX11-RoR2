#include "ClientPCH.h"
#include "Golem.h"
#include "Golem_Behavior.h"
#include "GameInstance.h"

CGolem::CGolem(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CGolem::CGolem(const CGolem& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CGolem::Initialize_Prototype()
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
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_GOLEM, g_aNull);

	m_umapBehaviorArg[BEHAVIOR::GROUNDING]	= make_pair(wstring(), wstring(GRID_TERRAIN));

	m_tEntityDesc.fForwardSpeed				= GOLEM_SPEED_FORWARD;
	m_tEntityDesc.fBackwardSpeed			= GOLEM_SPEED_BACKWARD;
	m_tEntityDesc.fLeftSpeed				= GOLEM_SPEED_LEFT;
	m_tEntityDesc.fRightSpeed				= GOLEM_SPEED_RIGHT;
	m_tEntityDesc.fSpritPower				= GOLEM_SPRINT_POWER;
	m_tEntityDesc.fJumpPower				= GOLEM_JUMP_POWER;

	m_tEntityDesc.vMaxSpeed					= GOLEM_SPEED_TERMINAL;
	m_tEntityDesc.vResist					= GOLEM_SPEED_RESIST;

	return S_OK;
}

HRESULT CGolem::Initialize(any)
{
	if (FAILED(__super::Initialize("Eye")))
	{
		MSG_RETURN(E_FAIL, "CGolem::Initialize", "Failed to __super::Initialize");
	}

	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::GOLEM::IDLE);

	return S_OK;
}

void CGolem::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (CGameInstance::Get_Instance()->Key_Down('Q'))
	{
		m_pAnimator->Play_Animation(ANIMATION::GOLEM::HIT_LIGHTIMPACT, 2.f, false, g_fDefaultInterpolationDuration, false);
	}
}

void CGolem::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CGolem::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CGolem::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CGolem::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CGolem::Ready_Components", "Failed to __super::Ready_Components");
	}

	return S_OK;
}

HRESULT CGolem::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CGolem::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_umapBehavior.emplace(BEHAVIOR::CUSTOM, CGolem_Behavior::Create(shared_from_gameobject(), Function::Find_Player(), &m_tEntityDesc));

	return S_OK;
}

void CGolem::Wander()
{
	_float	fProgressTime(Function::RandomFloat(GOLEM_MOVEMENT_TIME_MIN, GOLEM_MOVEMENT_TIME_MAX));
	_float3	vLook(Function::RandomFloat(-1.f, 1.f), 0.f, Function::RandomFloat(-1.f, 1.f));

	m_pTransform->LookTo_Interpolation(vLook);
	m_pAnimator->Play_Animation(ANIMATION::GOLEM::WALK, 1.5f);

	_float fAcc(0.f);
	CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 1.f)
			{
				fAcc += _fTimeDelta / fProgressTime;
				m_pPhysics->Force(TRANSFORM::LOOK, m_tEntityDesc.fForwardSpeed, _fTimeDelta);
				return true;
			}
			else
			{
				m_pAnimator->Play_Animation(ANIMATION::GOLEM::IDLE);
				return false;
			}
		}
	);
}

void CGolem::Hit()
{
	m_pAnimator->Play_Animation(ANIMATION::GOLEM::HIT_LIGHTIMPACT, 2.f, false, g_fDefaultInterpolationDuration, false);
	Get_Behavior<CGolem_Behavior>(BEHAVIOR::CUSTOM)->Set_State(CGolem_Behavior::STATE::HIT);

	m_pPhysics->Force(TRANSFORM::LOOK, -m_tEntityDesc.fForwardSpeed * 0.25f);
}

void CGolem::Hit_WeakPoint()
{
	m_pAnimator->Play_Animation(ANIMATION::GOLEM::HIT_HURT1, 2.f, false, g_fDefaultInterpolationDuration, false);
	m_pAnimator->Reset();

	_float fAcc(0.f);
	CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (m_pAnimator->Is_Finished())
			{
				m_pAnimator->Play_Animation(ANIMATION::GOLEM::IDLE);

				return false;
			}
			else
			{
				return true;
			}
		}
	);

	Get_Behavior<CGolem_Behavior>(BEHAVIOR::CUSTOM)->Set_State(CGolem_Behavior::STATE::HIT);
}

shared_ptr<CGolem> CGolem::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CGolem> pInstance = make_private_shared(CGolem, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CGolem::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CGolem::Clone(any)
{
	shared_ptr<CGolem> pInstance = make_private_shared_copy(CGolem, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CGolem::Clone", "Failed to Initialize");
	}

	return pInstance;
}
