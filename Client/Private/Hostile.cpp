#include "ClientPCH.h"
#include "Hostile.h"
#include "GameInstance.h"
#include "GameObject.h"

CHostile::CHostile()
	: CBehavior(BEHAVIOR::CUSTOM)
{
}

CHostile::CHostile(const CHostile& _rhs)
	: CBehavior(_rhs)
{
}

HRESULT CHostile::Initialize(shared_ptr<CGameObject> _pOwner, shared_ptr<CGameObject> _pTarget, const ENTITYDESC* _pEntityDesc)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CHostile::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform = _pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == _pTarget)
	{
		MSG_RETURN(E_FAIL, "CHostile::Initialize", "Failed to Get_Component: TRANSFORM");
	}

	if (nullptr == _pEntityDesc)
	{
		MSG_RETURN(E_FAIL, "CHostile::Initialize", "Nullptr Exception: _pEntityDesc");
	}
	m_pEntityDesc = _pEntityDesc;

	if (nullptr == _pTarget)
	{
		MSG_RETURN(E_FAIL, "CHostile::Initialize", "Nullptr Exception: _pTarget");
	}
	m_pTarget = _pTarget;

	m_pTargetTransform = m_pTarget->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == _pTarget)
	{
		MSG_RETURN(E_FAIL, "CHostile::Initialize", "Failed to Get_Component: TRANSFORM");
	}

	return S_OK;
}

void CHostile::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CHostile::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CHostile::Render()
{


	return S_OK;
}

_bool CHostile::Is_Target_InRange(_float _fDistance) const
{
	return _fDistance > _float3(m_pTransform->Get_State(TRANSFORM::POSITION) - m_pTargetTransform->Get_State(TRANSFORM::POSITION)).length();
}

void CHostile::Chase_Target(_float _fTimeDelta)
{
	m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION));
	if (_float3(m_pTargetTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION)).length() < 5.f)
	{
		return;
	}
	
	m_pPhysics->Force(TRANSFORM::LOOK, m_pEntityDesc->fForwardSpeed, _fTimeDelta);
}
