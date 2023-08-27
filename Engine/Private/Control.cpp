#include "EnginePCH.h"
#include "Control.h"
#include "GameInstance.h"
#include "GameObject.h"

CControl::CControl()
	: CBehavior(BEHAVIOR::CONTROL)
{
}

CControl::CControl(const CControl& _rhs)
	: CBehavior			(_rhs)
	, m_pCharacterDesc	(_rhs.m_pCharacterDesc)
{
}

HRESULT CControl::Initialize(shared_ptr<CGameObject> _pOwner, const CHARACTERDESC* _pCharacterDesc)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "Failed to __super::Initialize");
	}

	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "weak_ptr Expired: m_pOwner");
	}

	shared_ptr<CTransform> pTargetTransform = m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "Failed to Get_Component");
	}

	shared_ptr<CPhysics> pTargetPhysics = m_pOwner.lock()->Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "Failed to Get_Component");
	}

	shared_ptr<CAnimator> pTargetAnimator = m_pOwner.lock()->Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR);
	if (nullptr == pTargetAnimator)
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "Failed to Get_Component");
	}

	m_pTargetTransform	= pTargetTransform;
	m_pTargetPhysics	= pTargetPhysics;
	m_pTargetAnimator	= pTargetAnimator;
	m_pCharacterDesc	= _pCharacterDesc;
	
	return S_OK;
}

void CControl::Tick(_float _fTimeDelta)
{
	if (nullptr == m_pTargetTransform)
	{
		return;
	}

	Handle_MouseInput(_fTimeDelta);
	Handle_KeyInput(_fTimeDelta);
}

void CControl::Late_Tick(_float _fTimeDelta)
{
}

void CControl::Handle_MouseInput(_float _fTimeDelta)
{
	POINT ptCursorMove{};

	if (!CGameInstance::Get_Instance()->Is_CursorOn() || CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_MOUSE_CONTROL))
	{
		ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();
	}

	if (ptCursorMove.x)
	{
		m_pTargetTransform->Rotate(_float3(0.f, 1.f, 0.f), MAINCAM_SENSITIVITY_YAW * ptCursorMove.x * _fTimeDelta);
	}

	if (ptCursorMove.y)
	{
		_float3	vLook		= m_pTargetTransform->Get_State(TRANSFORM::LOOK);
		_float	fCurPitch	= atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
		_float	fChgPitch	= MAINCAM_SENSITIVITY_PITCH * ptCursorMove.y * _fTimeDelta;
		_float	fNewPitch	= Function::Clamp(XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX), fCurPitch + fChgPitch);
		_float	fFinal		= fNewPitch - fCurPitch;

		m_pTargetTransform->Rotate(m_pTargetTransform->Get_State(TRANSFORM::RIGHT), fFinal);
	}
}

void CControl::Handle_KeyInput(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Up())
	{
		Refresh_Animations();
	}

#pragma region Move
#pragma region Forward
	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_FORWARD))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::SPRINT_FORWARD));
			}
		}
		else
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_FORWARD));
			}
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_FORWARD))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetPhysics)
			{
				m_pTargetPhysics->Force(TRANSFORM::LOOK, m_pCharacterDesc->fForwardSpeed * 1.5f, _fTimeDelta);
			}
			else if (m_pTargetTransform)
			{
				m_pTargetTransform->Translate(m_pTargetTransform->Get_State(TRANSFORM::LOOK).normalize() * m_pCharacterDesc->fForwardSpeed * 1.5f * _fTimeDelta);
			}
		}
		else
		{
			if (m_pTargetPhysics)
			{
				m_pTargetPhysics->Force(TRANSFORM::LOOK, m_pCharacterDesc->fForwardSpeed, _fTimeDelta);
			}
			else if (m_pTargetTransform)
			{
				m_pTargetTransform->Translate(m_pTargetTransform->Get_State(TRANSFORM::LOOK).normalize() * m_pCharacterDesc->fForwardSpeed * _fTimeDelta);
			}
		}
	}
#pragma endregion
#pragma region Backward

	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_BACKWARD))
	{
		if (m_pTargetAnimator)
		{
			m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_BACKWARD));
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_BACKWARD))
	{
		if (m_pTargetPhysics)
		{
			m_pTargetPhysics->Force(TRANSFORM::LOOK, -m_pCharacterDesc->fBackwardSpeed, _fTimeDelta);
		}
		else if (m_pTargetTransform)
		{
			m_pTargetTransform->Translate(-m_pTargetTransform->Get_State(TRANSFORM::LOOK).normalize() * m_pCharacterDesc->fBackwardSpeed * _fTimeDelta);
		}
	}
#pragma endregion
#pragma region Left
	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_LEFT))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::SPRINT_LEFT));
			}
		}
		else
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_LEFT));
			}
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_LEFT))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetPhysics)
			{
				m_pTargetPhysics->Force(TRANSFORM::RIGHT, -m_pCharacterDesc->fLeftSpeed * 1.5f, _fTimeDelta);
			}
			else if (m_pTargetTransform)
			{
				m_pTargetTransform->Translate(-m_pTargetTransform->Get_State(TRANSFORM::RIGHT).normalize() * m_pCharacterDesc->fLeftSpeed * 1.5f * _fTimeDelta);
			}
		}
		else
		{
			if (m_pTargetPhysics)
			{
				m_pTargetPhysics->Force(TRANSFORM::RIGHT, -m_pCharacterDesc->fLeftSpeed, _fTimeDelta);
			}
			else if (m_pTargetTransform)
			{
				m_pTargetTransform->Translate(-m_pTargetTransform->Get_State(TRANSFORM::RIGHT).normalize() * m_pCharacterDesc->fLeftSpeed * _fTimeDelta);
			}
		}
		
	}
#pragma endregion
#pragma region Right
	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_RIGHT))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::SPRINT_RIGHT));
			}
		}
		else
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_RIGHT));
			}
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_RIGHT))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetPhysics)
			{
				m_pTargetPhysics->Force(TRANSFORM::RIGHT, m_pCharacterDesc->fRightSpeed * 1.5f, _fTimeDelta);
			}
			else if (m_pTargetTransform)
			{
				m_pTargetTransform->Translate(m_pTargetTransform->Get_State(TRANSFORM::RIGHT).normalize() * m_pCharacterDesc->fRightSpeed * 1.5f * _fTimeDelta);
			}
		}
		else
		{
			if (m_pTargetPhysics)
			{
				m_pTargetPhysics->Force(TRANSFORM::RIGHT, m_pCharacterDesc->fRightSpeed, _fTimeDelta);
			}
			else if (m_pTargetTransform)
			{
				m_pTargetTransform->Translate(m_pTargetTransform->Get_State(TRANSFORM::RIGHT).normalize() * m_pCharacterDesc->fRightSpeed * _fTimeDelta);
			}
		}
	}
#pragma endregion
#pragma region Jump
	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_JUMP))
	{
		if (m_pTargetPhysics)
		{
			m_pTargetPhysics->Force(_float3(0.f, 1.f, 0.f), m_pCharacterDesc->fJumpPower);
		}
	}
#pragma endregion
}

void CControl::Refresh_Animations()
{
#pragma region Move
#pragma region Forward
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_FORWARD))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::SPRINT_FORWARD));
			}
		}
		else
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_FORWARD));
			}
		}
	}
#pragma endregion
#pragma region Backward
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_BACKWARD))
	{
		if (m_pTargetAnimator)
		{
			m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_BACKWARD));
		}
	}
#pragma endregion
#pragma region Left
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_LEFT))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::SPRINT_LEFT));
			}
		}
		else
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_LEFT));
			}
		}
	}
#pragma endregion
#pragma region Right
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_RIGHT))
	{
		if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_SPRINT))
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::SPRINT_RIGHT));
			}
		}
		else
		{
			if (m_pTargetAnimator)
			{
				m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::MOVE::RUN_RIGHT));
			}
		}
	}
#pragma endregion
#pragma region Jump
	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_JUMP))
	{
		if (m_pTargetPhysics)
		{
			m_pTargetPhysics->Force(_float3(0.f, 1.f, 0.f), m_pCharacterDesc->fJumpPower);
		}
	}
#pragma endregion

	if (!CGameInstance::Get_Instance()->Key_Hold())
	{
		if (m_pTargetAnimator)
		{
			m_pTargetAnimator->Play_Animation(IDX(ANIMATION::PLAYER::IDLE::IDLE));
		}
	}
}
 
shared_ptr<CControl> CControl::Create(shared_ptr<CGameObject> _pOwner, const CHARACTERDESC* _pCharacterDesc)
{
	shared_ptr<CControl> pInstance = make_private_shared(CControl);

	if (FAILED(pInstance->Initialize(_pOwner, _pCharacterDesc)))
	{
		MSG_RETURN(nullptr, "CControl::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CControl::Clone(any)
{
	return make_private_shared_copy(CControl, *this);
}
