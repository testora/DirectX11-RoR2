#include "EnginePCH.h"
#include "Transform.h"

CTransform::CTransform(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::TRANSFORM)
{
	m_mWorld = XMMatrixIdentity();
}

CTransform::CTransform(const CTransform& _rhs)
	: CComponent(_rhs)
	, m_mWorld	(_rhs.m_mWorld)
{
}

_float4x4 CTransform::Get_Matrix() const
{
	return m_mWorld;
}

_float3 CTransform::Get_State(const STATE _eState) const
{
	return *reinterpret_cast<const _float3*>(&m_mWorld.m[_eState][0]);
}

_float3 CTransform::Get_Scale() const
{
	return _float3(
		XMVectorGetX(XMVector3Length(Get_State(STATE::RIGHT))),
		XMVectorGetY(XMVector3Length(Get_State(STATE::UP))),
		XMVectorGetZ(XMVector3Length(Get_State(STATE::LOOK)))
	);
}

void CTransform::Set_State(const STATE _eState, const _vectorf _vState)
{
	_matrix mWorld(m_mWorld);

	if (Function::InRange(_eState, STATE::RIGHT, STATE::LOOK, "[]"))
	{
		m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[_eState], _vState));
	}
	else if(STATE::POSITION == _eState)
	{
		mWorld.r[_eState] = _vState;
		m_mWorld = mWorld;
	}
	else
	{
		return;
	}
}

void CTransform::Set_Scale(const _vectorf _vScale)
{
	_float3 vScaleFactor = _vScale / Get_Scale();
	
	m_mWorld *= XMMatrixScalingFromVector(vScaleFactor);
}

void CTransform::Multiply(const _matrixf _mMatrix)
{
	m_mWorld *= _mMatrix;
}

void CTransform::Translate(const _vectorf _vPosition)
{
	m_mWorld *= XMMatrixTranslationFromVector(_vPosition);
}

void CTransform::Rotate(const _vectorf _vAxis, const _float _fDegree)
{
	m_mWorld *= XMMatrixRotationQuaternion(XMQuaternionRotationAxis(_vAxis, XMConvertToRadians(_fDegree)));
}

void CTransform::LookAt(const _vectorf _vPosition)
{
	m_mWorld = XMMatrixLookAtLH(Get_State(STATE::POSITION), _vPosition, XMVectorSet(0.f, 1.f, 0.f, 0.f)) * XMMatrixScalingFromVector(Get_Scale());
}

void CTransform::LookTo(const _vectorf _vDirection)
{
	m_mWorld = XMMatrixLookToLH(Get_State(STATE::POSITION), _vDirection, XMVectorSet(0.f, 1.f, 0.f, 0.f)) * XMMatrixScalingFromVector(Get_Scale());
}

shared_ptr<CTransform> CTransform::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	return make_private_shared(CTransform, _pDevice, _pContext);
}

shared_ptr<CComponent> CTransform::Clone(any _arg)
{
	return make_private_shared_copy(CTransform, *this);
}
