#include "EnginePCH.h"
#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"

CPhysics::CPhysics(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CBehavior(_pDevice, _pContext, BEHAVIOR::PHYSICS)
{
}

CPhysics::CPhysics(const CPhysics& _rhs)
	: CBehavior		(_rhs)
	, m_vDirection	(_rhs.m_vDirection)
	, m_vMaxSpeed	(_rhs.m_vMaxSpeed)
	, m_vResist		(_rhs.m_vResist)
{
}

HRESULT CPhysics::Initialize(shared_ptr<CGameObject> _pOwner)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CPhysics::Initialize", "Failed to __super::Initialize");
	}

	shared_ptr<CTransform> pTargetTransform = m_pOwner->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CPhysics::Initialize", "Failed to Get_Component");
	}

	m_pTargetTransform = pTargetTransform;

	return S_OK;
}

void CPhysics::Tick(_float _fTimeDelta)
{
	Terminate();
	Resist(_fTimeDelta);

	m_pTargetTransform->Translate(m_vDirection);
}

void CPhysics::Late_Tick(_float _fTimeDelta)
{
}

void CPhysics::Force(_vectorf _vDirection, _float _fMagnitude, _float _fTimeDelta)
{
	m_vDirection += XMVector3Normalize(_vDirection) * _fMagnitude * _fTimeDelta;
}

void CPhysics::Force(TRANSFORM _eDirection, _float _fMagnitude, _float _fTimeDelta)
{
	if (Function::InRange(_eDirection, TRANSFORM::RIGHT, TRANSFORM::MAX))
	{
		Force(m_pTargetTransform->Get_State(_eDirection), _fMagnitude, _fTimeDelta);
	}
}

void CPhysics::Terminate()
{
	m_vDirection = XMVectorClamp(m_vDirection, -m_vMaxSpeed, m_vMaxSpeed);
}

void CPhysics::Resist(_float _fTimeDelta)
{
	m_vDirection.x *= powf(m_vResist.x, _fTimeDelta);
	m_vDirection.y *= powf(m_vResist.y, _fTimeDelta);
	m_vDirection.z *= powf(m_vResist.z, _fTimeDelta);
}

shared_ptr<CPhysics> CPhysics::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, shared_ptr<CGameObject> _pOwner)
{
	shared_ptr<CPhysics> pInstance = make_private_shared(CPhysics, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pOwner)))
	{
		MSG_RETURN(nullptr, "CPhysics::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CPhysics::Clone(any)
{
	return make_private_shared_copy(CPhysics, *this);
}
