#include "EnginePCH.h"
#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"

CPhysics::CPhysics(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CBehavior(_pDevice, _pContext, BEHAVIOR::PHYSICS)
{
}

CPhysics::CPhysics(const CPhysics& _rhs)
	: CBehavior			(_rhs)
	, m_vVelocity		(_rhs.m_vVelocity)
	, m_pCharacterDesc	(_rhs.m_pCharacterDesc)
{
}

HRESULT CPhysics::Initialize(shared_ptr<CGameObject> _pOwner, const CHARACTERDESC* _pCharacterDesc)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CPhysics::Initialize", "Failed to __super::Initialize");
	}

	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CPhysics::Initialize", "weak_ptr Expired: m_pOwner");
	}

	shared_ptr<CTransform> pTargetTransform = m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CPhysics::Initialize", "Failed to Get_Component");
	}

	m_pTargetTransform	= pTargetTransform;
	m_pCharacterDesc	= _pCharacterDesc;

	return S_OK;
}

void CPhysics::Tick(_float _fTimeDelta)
{
	Execute_Gravity(_fTimeDelta);
	Resist(_fTimeDelta);
	Terminate();

	m_pTargetTransform->Translate(m_vVelocity * _fTimeDelta);
}

void CPhysics::Late_Tick(_float _fTimeDelta)
{
	if (Function::NearZero3(m_vVelocity))
	{
		m_vVelocity = _float3(0.f, 0.f, 0.f);
	}
}

void CPhysics::Force(_vectorf _vDirection, _float _fMagnitude, _float _fTimeDelta)
{
	m_vVelocity += XMVector3Normalize(_vDirection) * _fMagnitude * _fTimeDelta;
}

void CPhysics::Force(TRANSFORM _eDirection, _float _fMagnitude, _float _fTimeDelta)
{
	if (Function::InRange(_eDirection, TRANSFORM::RIGHT, TRANSFORM::MAX))
	{
		Force(m_pTargetTransform->Get_State(_eDirection), _fMagnitude, _fTimeDelta);
	}
}

void CPhysics::Flattern(_bool _bX, _bool _bY, _bool _bZ)
{
	if (_bX) m_vVelocity.x = 0.f;
	if (_bY) m_vVelocity.y = 0.f;
	if (_bZ) m_vVelocity.z = 0.f;
}

void CPhysics::Execute_Gravity(_float _fTimeDelta)
{
	Force(_float3(0.f, -1.f, 0.f), g_fGravity, _fTimeDelta);
}

void CPhysics::Resist(_float _fTimeDelta)
{
	m_vVelocity.x *= powf(m_pCharacterDesc->vResist.x, _fTimeDelta);
	m_vVelocity.y *= powf(m_pCharacterDesc->vResist.y, _fTimeDelta);
	m_vVelocity.z *= powf(m_pCharacterDesc->vResist.z, _fTimeDelta);
}

void CPhysics::Terminate()
{
	m_vVelocity = XMVectorClamp(m_vVelocity, -_float3(m_pCharacterDesc->vMaxSpeed), _float3(m_pCharacterDesc->vMaxSpeed));
}

shared_ptr<CPhysics> CPhysics::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, shared_ptr<CGameObject> _pOwner, const CHARACTERDESC* _pCharacterDesc)
{
	shared_ptr<CPhysics> pInstance = make_private_shared(CPhysics, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pOwner, _pCharacterDesc)))
	{
		MSG_RETURN(nullptr, "CPhysics::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CPhysics::Clone(any)
{
	return make_private_shared_copy(CPhysics, *this);
}
