#include "ClientPCH.h"
#include "Monster.h"
#include "GameInstance.h"
#include "Bone.h"

CMonster::CMonster(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CMonster::CMonster(const CMonster& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CMonster::Initialize(any _szWeakPoint)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CMonster::Initialize", "Failed to __super::Initialize");
	}

	m_mPivot = Get_Component<CModel>(COMPONENT::MODEL)->Get_Pivot();

	if (_szWeakPoint.has_value())
	{
		m_pWeakPointWorld = Get_Component<CModel>(COMPONENT::MODEL)->Get_Bone(any_cast<const _char*>(_szWeakPoint))->Get_CombinedTransformationPointer();
	}

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

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Components", "Failed to Get_Component: SHADER");
	}

	m_pModel = Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == m_pModel)
	{
		MSG_RETURN(E_FAIL, "CMonster::Ready_Components", "Failed to Get_Component: MODEL");
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

HRESULT CMonster::Fetch(any _vPosition3)
{
	if (_vPosition3.has_value())
	{
		if (nullptr != m_pTransform)
		{
			m_pTransform->Set_State(TRANSFORM::POSITION, any_cast<_float3>(_vPosition3));
		}
	}
	
	return S_OK;
}

_float4x4 CMonster::Get_WeakPoint() const
{
	return (m_pWeakPointWorld ? XMLoadFloat4x4(m_pWeakPointWorld) : g_mUnit) * m_mPivot * m_pTransform->Get_Matrix();
}
