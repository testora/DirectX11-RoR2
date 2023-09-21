#include "ClientPCH.h"
#include "Control_RailGunner.h"
#include "RailGunner.h"
#include "RailGunner_State.h"
#include "RailGunner_Crosshair.h"
#include "Camera_Main.h"
#include "GameInstance.h"

#define BITMASK_FORWARD			BIT(DIRECTION::FORWARD)
#define BITMASK_BACKWARD		BIT(DIRECTION::BACKWARD)
#define BITMASK_LEFT			BIT(DIRECTION::LEFT)
#define BITMASK_RIGHT			BIT(DIRECTION::RIGHT)

#define BITMASK_FORWARDLEFT		BIT(DIRECTION::FORWARD)		| BIT(DIRECTION::LEFT)
#define BITMASK_FORWARDRIGHT	BIT(DIRECTION::FORWARD)		| BIT(DIRECTION::RIGHT)
#define BITMASK_BACKWARDLEFT	BIT(DIRECTION::BACKWARD)	| BIT(DIRECTION::LEFT)
#define BITMASK_BACKWARDRIGHT	BIT(DIRECTION::BACKWARD)	| BIT(DIRECTION::RIGHT)

HRESULT CControl_RailGunner::Initialize(shared_ptr<CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(E_FAIL, "CControl_RailGunner::Initialize", "Failed to __super::Initialize");
	}

	m_pRailGunner = dynamic_pointer_cast<CRailGunner>(_pOwner);
	if (nullptr == m_pRailGunner)
	{
		MSG_RETURN(E_FAIL, "CControl_RailGunner::Initialize", "Nullptr Exception: m_pRailGunner");
	}

	return S_OK;
}

void CControl_RailGunner::Tick(_float _fTimeDelta)
{
	Handle_Bitset();

	__super::Tick(_fTimeDelta);
}

void CControl_RailGunner::Late_Tick(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_JUMP))
	{
		if (m_pRailGunner->Is_State(BIT(RG_STATE::AIR)))
		{
			return;
		}
		else
		{
			m_pTargetPhysics->Force(TRANSFORM::UP, m_pEntityDesc->fJumpPower);
			m_pTargetAnimator->Play_Animation(ANIMATION::RAILGUNNER::JUMP_START, 1.f, false, g_fDefaultInterpolationDuration, false);
		}
	}

	__super::Late_Tick(_fTimeDelta);
}

void CControl_RailGunner::Handle_MouseInput(_float _fTimeDelta)
{
	shared_ptr<CGameInstance>	pGameInstance			= CGameInstance::Get_Instance();
	shared_ptr<CCamera_Main>	pMainCamera				= CPipeLine::Get_Instance()->Get_Camera<CCamera_Main>();
	shared_ptr<CTransform>		pMainCameraTransform	= pMainCamera->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	switch (m_pRailGunner->Get_Crosshair())
	{
	case RG_CROSSHAIR::MAIN:
	{
		if (pGameInstance->Key_Down(CONTROL_ATTACK1))
		{
			if (m_pRailGunner->Is_State(BIT(RG_STATE::READY_PISTOL)))
			{
				m_pRailGunner->Bounce_Bracket();
				m_pRailGunner->Set_State(RG_STATE::READY_PISTOL, false);
				m_pRailGunner->Fire_Pistol();
				m_pTargetTransform->LookTo_Interpolation(pMainCameraTransform->Get_State(TRANSFORM::LOOK));
				m_pRailGunner->Set_State(RG_STATE::FIRE_PISTOL);
				pMainCamera->Rebound_Pistol();
			}
		}
		if (pGameInstance->Key_Hold(CONTROL_ATTACK1))
		{
			m_pRailGunner->Set_State(RG_STATE::AIM);

			if (m_pRailGunner->Is_State(BIT(RG_STATE::READY_PISTOL)))
			{
				m_pRailGunner->Bounce_Bracket();
				m_pRailGunner->Set_State(RG_STATE::READY_PISTOL, false);
				m_pRailGunner->Fire_Pistol();
				m_pTargetTransform->LookTo_Interpolation(pMainCameraTransform->Get_State(TRANSFORM::LOOK));
				m_pRailGunner->Set_State(RG_STATE::FIRE_PISTOL);
				pMainCamera->Rebound_Pistol();
			}
		}
		if (pGameInstance->Key_Up(CONTROL_ATTACK1))
		{
			m_pRailGunner->Set_State(RG_STATE::AIM, false);
		}
		if (pGameInstance->Key_Hold(CONTROL_ATTACK2))
		{
			m_pRailGunner->Set_State(RG_STATE::READY_SNIPER);
			m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::SCOPE);
			m_pTargetTransform->LookTo_Interpolation(pMainCameraTransform->Get_State(TRANSFORM::LOOK));
			pMainCamera->Adjust_FOV(RAILGUNNER_SCOPE_FOV, RAILGUNNER_SCOPE_ZOOM_IN_DURATION, RAILGUNNER_SCOPE_ZOOM_WEIGHT,
				_float2(RAILGUNNER_SCOPE_SENSITIVITY_YAW, RAILGUNNER_SCOPE_SENSITIVITY_PITCH));
		}
	}
	break;
	case RG_CROSSHAIR::SCOPE:
	{
		if (pGameInstance->Key_Down(CONTROL_ATTACK1))
		{
			if (m_pRailGunner->Is_State(BIT(RG_STATE::READY_SNIPER)))
			{
				m_pRailGunner->Fire_Sniper();
				m_pRailGunner->Set_State(RG_STATE::READY_SNIPER, false);
				m_pRailGunner->Set_State(RG_STATE::READY_RELOAD);
				pMainCamera->Rebound_Sniper();
			}
		}
		if (pGameInstance->Key_Up(CONTROL_ATTACK2))
		{
			if (!m_pRailGunner->Is_State(BIT(RG_STATE::READY_RELOAD)))
			{
				m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::MAIN);
			}
			if (m_pRailGunner->Is_State(BIT(RG_STATE::READY_RELOAD)))
			{
				m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::RELOAD);
			}
			pMainCamera->Release_FOV(RAILGUNNER_SCOPE_ZOOM_OUT_DURATION, RAILGUNNER_SCOPE_ZOOM_WEIGHT);
		}
	}
	break;
	case RG_CROSSHAIR::SUPER_CHARGE:
	{

	}
	break;
	case RG_CROSSHAIR::SUPER_READY:
	{

	}
	break;
	case RG_CROSSHAIR::SUPER_REBOOT:
	{

	}
	break;
	case RG_CROSSHAIR::RELOAD:
	{
		if (pGameInstance->Key_Down(CONTROL_ATTACK1))
		{
			m_pRailGunner->Hit_Reload();
			m_pRailGunner->Set_State(RG_STATE::READY_RELOAD, false);
		}
	}
	break;
	case RG_CROSSHAIR::SPRINT:
	{
		if (!m_pRailGunner->Is_State(BIT(RG_STATE::SPRINT)))
		{
			
		}
	}
	break;
	}
}

void CControl_RailGunner::Handle_KeyInput(_float _fTimeDelta)
{
	shared_ptr<CGameInstance>	pGameInstance			= CGameInstance::Get_Instance();
	shared_ptr<CPipeLine>		pPipeLine				= CPipeLine::Get_Instance();
	_bool						bIsAim					= m_pRailGunner->Is_State(BIT(RG_STATE::AIM));
	_bool						bIsAir					= m_pRailGunner->Is_State(BIT(RG_STATE::AIR));
	_bool						bIsSprint				= m_pRailGunner->Is_State(BIT(RG_STATE::SPRINT));

	switch (static_cast<_uint>(m_bitDirection.to_ulong()))
	{
	case BITMASK_FORWARD:
	{
		if (pGameInstance->Key_Down(CONTROL_SPRINT))
		{
			m_pRailGunner->Set_State(RG_STATE::SPRINT);
		}

		m_pTargetTransform->LookTo_Interpolation(pPipeLine->Get_Transform(TRANSFORM::LOOK));
		m_pTargetPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed * (bIsSprint ? m_pEntityDesc->fSpritPower : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_FORWARD : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_BACKWARD:
	{
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : -pPipeLine->Get_Transform(TRANSFORM::LOOK));
		m_pTargetPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fBackwardSpeed * (bIsAim ? -1.f : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsAim ? ANIMATION::RAILGUNNER::RUN_BACKWARD : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_LEFT:
	{
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : -pPipeLine->Get_Transform(TRANSFORM::RIGHT));
		m_pTargetPhysics->Force(bIsAim ? TRANSFORM::RIGHT : TRANSFORM::LOOK, m_pEntityDesc->fLeftSpeed * (bIsAim ? -1.f : 1.f) * (bIsSprint ? m_pEntityDesc->fSpritPower : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_LEFT : bIsAim ? ANIMATION::RAILGUNNER::RUN_LEFT : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_RIGHT:
	{
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : pPipeLine->Get_Transform(TRANSFORM::RIGHT));
		m_pTargetPhysics->Force(bIsAim ? TRANSFORM::RIGHT : TRANSFORM::LOOK, m_pEntityDesc->fRightSpeed * (bIsSprint ? m_pEntityDesc->fSpritPower : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_RIGHT : bIsAim ? ANIMATION::RAILGUNNER::RUN_RIGHT : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_FORWARDLEFT:
	{
		if (pGameInstance->Key_Down(CONTROL_SPRINT))
		{
			m_pRailGunner->Set_State(RG_STATE::SPRINT);
		}

		_float4 vDirection = pPipeLine->Get_Transform(TRANSFORM::LOOK) - pPipeLine->Get_Transform(TRANSFORM::RIGHT);
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : vDirection);
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pEntityDesc->fForwardSpeed * (bIsSprint ? m_pEntityDesc->fSpritPower : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_LEFT : bIsAim ? ANIMATION::RAILGUNNER::RUN_LEFT : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_FORWARDRIGHT:
	{
		if (pGameInstance->Key_Down(CONTROL_SPRINT))
		{
			m_pRailGunner->Set_State(RG_STATE::SPRINT);

		}
		_float4 vDirection = pPipeLine->Get_Transform(TRANSFORM::LOOK) + pPipeLine->Get_Transform(TRANSFORM::RIGHT);
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : vDirection);
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pEntityDesc->fForwardSpeed * (bIsSprint ? m_pEntityDesc->fSpritPower : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_RIGHT : bIsAim ? ANIMATION::RAILGUNNER::RUN_RIGHT : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_BACKWARDLEFT:
	{
		_float4 vDirection = -pPipeLine->Get_Transform(TRANSFORM::LOOK) - pPipeLine->Get_Transform(TRANSFORM::RIGHT);
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : vDirection);
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pEntityDesc->fForwardSpeed, _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_LEFT : bIsAim ? ANIMATION::RAILGUNNER::RUN_LEFT : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_BACKWARDRIGHT:
	{
		_float4 vDirection = -pPipeLine->Get_Transform(TRANSFORM::LOOK) + pPipeLine->Get_Transform(TRANSFORM::RIGHT);
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : vDirection);
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pEntityDesc->fForwardSpeed, _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_RIGHT : bIsAim ? ANIMATION::RAILGUNNER::RUN_RIGHT : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;

	default:
		m_pTargetAnimator->Play_Animation(ANIMATION::RAILGUNNER::IDLE);
		break;
	}
}

void CControl_RailGunner::Handle_Bitset()
{
	shared_ptr<CGameInstance> pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down(CONTROL_FORWARD))
	{
		m_bitDirection.set(IDX(DIRECTION::FORWARD));
	}
	else if (pGameInstance->Key_Up(CONTROL_FORWARD))
	{
		m_bitDirection.set(IDX(DIRECTION::FORWARD), false);
	}
	if (pGameInstance->Key_Down(CONTROL_BACKWARD))
	{
		m_bitDirection.set(IDX(DIRECTION::BACKWARD));
	}
	else if (pGameInstance->Key_Up(CONTROL_BACKWARD))
	{
		m_bitDirection.set(IDX(DIRECTION::BACKWARD), false);
	}
	if (pGameInstance->Key_Down(CONTROL_LEFT))
	{
		m_bitDirection.set(IDX(DIRECTION::LEFT));
	}
	else if (pGameInstance->Key_Up(CONTROL_LEFT))
	{
		m_bitDirection.set(IDX(DIRECTION::LEFT), false);
	}
	if (pGameInstance->Key_Down(CONTROL_RIGHT))
	{
		m_bitDirection.set(IDX(DIRECTION::RIGHT));
	}
	else if (pGameInstance->Key_Up(CONTROL_RIGHT))
	{
		m_bitDirection.set(IDX(DIRECTION::RIGHT), false);
	}
}

shared_ptr<CControl_RailGunner> CControl_RailGunner::Create(shared_ptr<class CGameObject> _pOwner, const ENTITYDESC* _pEntityDesc)
{
	shared_ptr<CControl_RailGunner> pInstance = make_private_shared(CControl_RailGunner);

	if (FAILED(pInstance->Initialize(_pOwner, _pEntityDesc)))
	{
		MSG_RETURN(nullptr, "CControl_RailGunner::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CControl_RailGunner::Clone(any)
{
	return make_private_shared_copy(CControl_RailGunner, *this);
}
