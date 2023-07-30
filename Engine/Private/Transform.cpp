#include "EnginePCH.h"
#include "Transform.h"

CTransform::CTransform(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::TRANSFORM)
{
	m_mWorld = XMMatrixIdentity();
}

_float4x4 CTransform::Get_Matrix() const
{
	return m_mWorld;
}

_float3 CTransform::Get_State(const TRANSFORM _eState) const
{
	return *reinterpret_cast<const _float3*>(&m_mWorld.m[IDX(_eState)][0]);
}

_float3 CTransform::Get_Scale() const
{
	return _float3(
		XMVectorGetX(XMVector3Length(Get_State(TRANSFORM::RIGHT))),
		XMVectorGetY(XMVector3Length(Get_State(TRANSFORM::UP))),
		XMVectorGetZ(XMVector3Length(Get_State(TRANSFORM::LOOK)))
	);
}

void CTransform::Set_State(const TRANSFORM _eState, const _float3 _vState)
{
	_matrix mWorld(m_mWorld);

	m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], _vState));
}

void CTransform::Set_Scale(const _float3 _vScale)
{
	_float3 vScaleFactor = _vScale / Get_Scale();
	
	m_mWorld *= XMMatrixScalingFromVector(vScaleFactor);
}

void CTransform::Multiply(const _float4x4 _mMatrix)
{
	m_mWorld *= _mMatrix;
}

void CTransform::Translate(const _float3 _vPosition)
{
	m_mWorld *= XMMatrixTranslationFromVector(_vPosition);
}

void CTransform::Rotate(const _float3 _vAxis, const _float _fDegree)
{
	m_mWorld *= XMMatrixRotationQuaternion(XMQuaternionRotationAxis(_vAxis, XMConvertToRadians(_fDegree)));
}

void CTransform::LookAt(const _float3 _vPosition)
{
	m_mWorld = XMMatrixLookAtLH(Get_State(TRANSFORM::POSITION), _vPosition, XMVectorSet(0.f, 1.f, 0.f, 0.f)) * XMMatrixScalingFromVector(Get_Scale());
}

void CTransform::LookTo(const _float3 _vDirection)
{
	m_mWorld = XMMatrixLookToLH(Get_State(TRANSFORM::POSITION), _vDirection, XMVectorSet(0.f, 1.f, 0.f, 0.f)) * XMMatrixScalingFromVector(Get_Scale());
}
