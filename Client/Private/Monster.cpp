#include "ClientPCH.h"
#include "Monster.h"
#include "GameInstance.h"

CMonster::CMonster(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMonster::CMonster(const CMonster& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CMonster::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster::Initialize", "Failed to __super::Initialize");
	}

	m_mPivot = Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();

	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Components", "Failed to Get_Component: TRANSFORM");
	}

	return S_OK;
}

HRESULT CMonster::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_pPhysics = Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Behaviors", "Failed to Get_Behavior: PHYSICS");
	}

	m_pAnimator = Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR);
	if (nullptr == m_pAnimator)
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Behaviors", "Failed to Get_Behavior: ANIMATOR");
	}

	return S_OK;
}

_float4x4 CMonster::Get_WeakPoint() const
{
	return *m_pWeakPointWorld * m_mPivot * m_pTransform->Get_Matrix();
}

_float CMonster::Distance(shared_ptr<CGameObject> _pObject)
{
	shared_ptr<CTransform> pTargetTransform = _pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == pTargetTransform)
	{
		MSG_RETURN(FLT_MAX, "CMonster::Discance", "Failed to Get_Component: TRANSFORM");
	}

	return _float3(m_pTransform->Get_State(TRANSFORM::POSITION) - pTargetTransform->Get_State(TRANSFORM::POSITION)).length();
}
