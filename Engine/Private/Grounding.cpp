#include "EnginePCH.h"
#include "Grounding.h"
#include "Grid_Manager.h"
#include "ObjectLayer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"

#define CRITERION_GROUND					1.f
#define CRITERION_OBSTACLE_VERTICAL			_float3(0.f, 1.f, 0.f)
#define CRITERION_OBSTACLE_SCALE			g_fTolorance	//10.f
#define CRITERION_OBSTACLE_SAFE_DISTANCE	2.f
#define REFLECT_SCALE						2.f

CGrounding::CGrounding()
	: CBehavior(BEHAVIOR::GROUNDING)
{
}

CGrounding::CGrounding(const CGrounding& _rhs)
	: CBehavior(_rhs)
{
}

HRESULT CGrounding::Initialize(shared_ptr<CGameObject> _pOwner, const wstring& _wstrTerrainGridLayerTag)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CGrounding::Initialize", "Failed to __super::Initialize");
	}

	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CGrounding::Initialize", "weak_ptr Expired: m_pOwner");
	}

	shared_ptr<CTransform> pOwnerTransform = m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == pOwnerTransform)
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "Failed to Get_Component: CTransform");
	}

	shared_ptr<CPhysics> pOwnerPhysics = m_pOwner.lock()->Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == pOwnerPhysics)
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "Failed to Get_Behavior: CPhysics");
	}

	m_pOwnerTransform			= pOwnerTransform;
	m_pOwnerPhysics				= pOwnerPhysics;
	m_wstrTerrainGridLayerTag	= _wstrTerrainGridLayerTag;

	return S_OK;
}

void CGrounding::Tick(_float _fTimeDelta)
{
	_float3 vGroundPosition = Intersect_Terrain_Ground();
	if (vGroundPosition == m_pOwnerTransform->Get_State(TRANSFORM::POSITION) + _float3(0.f, CRITERION_GROUND, 0.f))
	{
		m_bIsGrounding = false;
		return;
	}

	m_pOwnerTransform->Set_State(TRANSFORM::POSITION, _float4(vGroundPosition, 1.f));
	m_pOwnerPhysics->Flattern(false, true, false);
	m_bIsGrounding = true;
}

void CGrounding::Late_Tick(_float _fTimeDelta)
{
	m_bIsObstacleHit = false;

	if (m_pOwnerPhysics->Is_Moving(true, false, true))
	{
		_float3 vNormal = _float3{};
		_float3 vObstaclePosition = Intersect_Terrain_Obstacle(_fTimeDelta, &vNormal);

		_float3 vHorizontalDelta = m_pOwnerPhysics->Get_Velocity() * _fTimeDelta;
		vHorizontalDelta.y = 0.f;

		if (vObstaclePosition != m_pOwnerTransform->Get_State(TRANSFORM::POSITION) - vHorizontalDelta * CRITERION_OBSTACLE_SCALE + CRITERION_OBSTACLE_VERTICAL)
		{
			_float3 vReflection = Function::ComputeSlideVector(m_pOwnerPhysics->Get_Velocity(), vNormal, true);
			m_pOwnerPhysics->Force(-vReflection);
			m_pOwnerTransform->Translate(-vReflection * (_fTimeDelta));
			m_bIsObstacleHit = true;
		}
	}
}

_float3 CGrounding::Intersect_Terrain_Ground()
{
	return CGrid_Manager::Get_Instance()->Raycast(m_wstrTerrainGridLayerTag,
		m_pOwnerTransform->Get_State(TRANSFORM::POSITION) + _float3(0.f, CRITERION_GROUND, 0.f), _float3(0.f, -1.f, 0.f), CRITERION_GROUND);
}

_float3 CGrounding::Intersect_Terrain_Obstacle(_float _fTimeDelta, _float3* _pNormal)
{
	_float3 vHorizontalDelta = m_pOwnerPhysics->Get_Velocity() * _fTimeDelta;
	vHorizontalDelta.y = 0.f;

	return CGrid_Manager::Get_Instance()->Raycast(m_wstrTerrainGridLayerTag,
		m_pOwnerTransform->Get_State(TRANSFORM::POSITION) - vHorizontalDelta * CRITERION_OBSTACLE_SCALE + CRITERION_OBSTACLE_VERTICAL, vHorizontalDelta, vHorizontalDelta.length() * CRITERION_OBSTACLE_SCALE + CRITERION_OBSTACLE_SAFE_DISTANCE, _pNormal);
}

shared_ptr<CGrounding> CGrounding::Create(shared_ptr<CGameObject> _pOwner, const wstring& _wstrTerrainGridLayerTag)
{
	shared_ptr<CGrounding> pInstance = make_private_shared(CGrounding);

	if (FAILED(pInstance->Initialize(_pOwner, _wstrTerrainGridLayerTag)))
	{
		MSG_RETURN(nullptr, "CGrounding::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CGrounding::Clone(any)
{
	return make_private_shared_copy(CGrounding, *this);
}
