#include "EnginePCH.h"
#include "Control.h"
#include "GameInstance.h"
#include "GameObject.h"

CControl::CControl()
	: CBehavior(BEHAVIOR::CONTROL)
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

