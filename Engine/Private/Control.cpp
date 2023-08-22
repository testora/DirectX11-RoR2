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

	m_pTargetTransform	= pTargetTransform;
	m_pTargetPhysics	= pTargetPhysics;
	m_pCharacterDesc	= _pCharacterDesc;
	
	return S_OK;
}

void CControl::Tick(_float _fTimeDelta)
{
	Handle_Input(_fTimeDelta);
}

void CControl::Late_Tick(_float _fTimeDelta)
{
}

void CControl::Handle_Input(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_FORWARD))
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
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_LEFT))
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
	if (CGameInstance::Get_Instance()->Key_Hold(CONTROL_RIGHT))
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
	if (CGameInstance::Get_Instance()->Key_Down(CONTROL_JUMP))
	{
		if (m_pTargetPhysics)
		{
			m_pTargetPhysics->Force(_float3(0.f, 1.f, 0.f), m_pCharacterDesc->fJumpPower);
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
