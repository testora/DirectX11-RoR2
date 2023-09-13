#include "ClientPCH.h"
#include "Control_RailGunner.h"
#include "RailGunner.h"
#include "RailGunner_State.h"
#include "RailGunner_Crosshair.h"
#include "Camera_Main.h"
#include "GameInstance.h"

#define BITMASK_FORWARD			BIT(STATE::FORWARD)
#define BITMASK_BACKWARD		BIT(STATE::BACKWARD)
#define BITMASK_LEFT			BIT(STATE::LEFT)
#define BITMASK_RIGHT			BIT(STATE::RIGHT)

#define BITMASK_FORWARDLEFT		BIT(STATE::FORWARD)		| BIT(STATE::LEFT)
#define BITMASK_FORWARDRIGHT	BIT(STATE::FORWARD)		| BIT(STATE::RIGHT)
#define BITMASK_BACKWARDLEFT	BIT(STATE::BACKWARD)	| BIT(STATE::LEFT)
#define BITMASK_BACKWARDRIGHT	BIT(STATE::BACKWARD)	| BIT(STATE::RIGHT)

HRESULT CControl_RailGunner::Initialize(shared_ptr<CGameObject> _pOwner, const CHARACTERDESC* _pCharacterDesc)
{
	if (FAILED(__super::Initialize(_pOwner, _pCharacterDesc)))
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
			m_pTargetPhysics->Force(TRANSFORM::UP, m_pCharacterDesc->fJumpPower);
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
	shared_ptr<CTransform>		pTransfrom				= m_pRailGunner->Get_Component<CTransform>(COMPONENT::TRANSFORM);

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
				pMainCamera->Rebound_Pistol();
			}
		}
		if (pGameInstance->Key_Up(CONTROL_ATTACK1))
		{
			m_pRailGunner->Set_State(RG_STATE::AIM, false);
		}
		if (pGameInstance->Key_Down(CONTROL_ATTACK2))
		{
			m_pRailGunner->Visualize_Crosshair(RG_CROSSHAIR::SCOPE);
			pTransfrom->LookTo_Interpolation(pMainCameraTransform->Get_State(TRANSFORM::LOOK));
			pMainCamera->Adjust_FOV(RAILGUNNER_SCOPE_FOV, RAILGUNNER_SCOPE_ZOOM_IN_DURATION, RAILGUNNER_SCOPE_ZOOM_WEIGHT);
		}
	}
	break;
	case RG_CROSSHAIR::SCOPE:
	{
		if (pGameInstance->Key_Down(CONTROL_ATTACK1))
		{
			m_pRailGunner->Set_State(RG_STATE::READY_SNIPER, false);
			m_pRailGunner->Set_State(RG_STATE::READY_RELOAD, true);
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

	switch (static_cast<_uint>(m_bitState.to_ulong()))
	{
	case BITMASK_FORWARD:
	{
		if (pGameInstance->Key_Down(CONTROL_SPRINT))
		{
			m_pRailGunner->Set_State(RG_STATE::SPRINT);
		}

		m_pTargetTransform->LookTo_Interpolation(pPipeLine->Get_Transform(TRANSFORM::LOOK));
		m_pTargetPhysics->Force(TRANSFORM::LOOK, m_pCharacterDesc->fForwardSpeed * (bIsSprint ? m_pCharacterDesc->fSpritPower : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_FORWARD : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_BACKWARD:
	{
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : -pPipeLine->Get_Transform(TRANSFORM::LOOK));
		m_pTargetPhysics->Force(TRANSFORM::LOOK, m_pCharacterDesc->fBackwardSpeed * (bIsAim ? -1.f : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsAim ? ANIMATION::RAILGUNNER::RUN_BACKWARD : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_LEFT:
	{
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : -pPipeLine->Get_Transform(TRANSFORM::RIGHT));
		m_pTargetPhysics->Force(bIsAim ? TRANSFORM::RIGHT : TRANSFORM::LOOK, m_pCharacterDesc->fLeftSpeed * (bIsAim ? -1.f : 1.f) * (bIsSprint ? m_pCharacterDesc->fSpritPower : 1.f), _fTimeDelta);
		if (!bIsAir)
		{
			m_pTargetAnimator->Play_Animation(bIsSprint ? ANIMATION::RAILGUNNER::SPRINT_LEFT : bIsAim ? ANIMATION::RAILGUNNER::RUN_LEFT : ANIMATION::RAILGUNNER::RUN_FORWARD);
		}
	}
	break;
	case BITMASK_RIGHT:
	{
		m_pTargetTransform->LookTo_Interpolation(bIsAim ? pPipeLine->Get_Transform(TRANSFORM::LOOK) : pPipeLine->Get_Transform(TRANSFORM::RIGHT));
		m_pTargetPhysics->Force(bIsAim ? TRANSFORM::RIGHT : TRANSFORM::LOOK, m_pCharacterDesc->fRightSpeed * (bIsSprint ? m_pCharacterDesc->fSpritPower : 1.f), _fTimeDelta);
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
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pCharacterDesc->fForwardSpeed * (bIsSprint ? m_pCharacterDesc->fSpritPower : 1.f), _fTimeDelta);
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
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pCharacterDesc->fForwardSpeed * (bIsSprint ? m_pCharacterDesc->fSpritPower : 1.f), _fTimeDelta);
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
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pCharacterDesc->fForwardSpeed, _fTimeDelta);
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
		m_pTargetPhysics->Force(bIsAim ? _float3(vDirection) : m_pTargetTransform->Get_State(TRANSFORM::LOOK), m_pCharacterDesc->fForwardSpeed, _fTimeDelta);
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
		m_bitState.set(IDX(STATE::FORWARD));
	}
	else if (pGameInstance->Key_Up(CONTROL_FORWARD))
	{
		m_bitState.set(IDX(STATE::FORWARD), false);
	}
	if (pGameInstance->Key_Down(CONTROL_BACKWARD))
	{
		m_bitState.set(IDX(STATE::BACKWARD));
	}
	else if (pGameInstance->Key_Up(CONTROL_BACKWARD))
	{
		m_bitState.set(IDX(STATE::BACKWARD), false);
	}
	if (pGameInstance->Key_Down(CONTROL_LEFT))
	{
		m_bitState.set(IDX(STATE::LEFT));
	}
	else if (pGameInstance->Key_Up(CONTROL_LEFT))
	{
		m_bitState.set(IDX(STATE::LEFT), false);
	}
	if (pGameInstance->Key_Down(CONTROL_RIGHT))
	{
		m_bitState.set(IDX(STATE::RIGHT));
	}
	else if (pGameInstance->Key_Up(CONTROL_RIGHT))
	{
		m_bitState.set(IDX(STATE::RIGHT), false);
	}
}

shared_ptr<CControl_RailGunner> CControl_RailGunner::Create(shared_ptr<class CGameObject> _pOwner, const CHARACTERDESC* _pCharacterDesc)
{
	shared_ptr<CControl_RailGunner> pInstance = make_private_shared(CControl_RailGunner);

	if (FAILED(pInstance->Initialize(_pOwner, _pCharacterDesc)))
	{
		MSG_RETURN(nullptr, "CControl_RailGunner::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CControl_RailGunner::Clone(any)
{
	return make_private_shared_copy(CControl_RailGunner, *this);
}
