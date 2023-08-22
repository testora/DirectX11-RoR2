#include "EnginePCH.h"
#include "Transform.h"

CTransform::CTransform(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::TRANSFORM)
	, m_mWorld	(g_mUnit)
{
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

void CTransform::Set_State(const TRANSFORM _eState, const _vectorf _vState)
{
	_matrix mWorld(m_mWorld);

	if (Function::InRange(_eState, TRANSFORM::RIGHT, TRANSFORM::LOOK, "[]"))
	{
		if (XMVector3Equal(XMVector3Cross(XMVector3Normalize(mWorld.r[IDX(_eState)]), XMVector3Normalize(_vState)), XMVectorZero()))
		{
			switch (_eState)
			{
			case TRANSFORM::RIGHT:
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::UP)]));
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::UP)]));
				break;
			case TRANSFORM::UP:
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::LOOK)]));
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::LOOK)]));
				break;
			case TRANSFORM::LOOK:
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::RIGHT)]));
				m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], mWorld.r[IDX(TRANSFORM::RIGHT)]));
				break;
			}
		}
		m_mWorld *= XMMatrixRotationQuaternion(QuaternionBetweenAxis(mWorld.r[IDX(_eState)], _vState));
		Set_State(TRANSFORM::POSITION, mWorld.r[IDX(TRANSFORM::POSITION)]);
	}
	else if(TRANSFORM::POSITION == _eState)
	{
		mWorld.r[IDX(_eState)] = _vState;
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
	_matrix mWorld(m_mWorld);
	m_mWorld *= XMMatrixRotationQuaternion(XMQuaternionRotationAxis(_vAxis, XMConvertToRadians(_fDegree)));
	Set_State(TRANSFORM::POSITION, mWorld.r[IDX(TRANSFORM::POSITION)]);
}

void CTransform::LookAt(const _vectorf _vPosition)
{
	Set_State(TRANSFORM::LOOK, _vPosition - Get_State(TRANSFORM::POSITION));
}

void CTransform::LookTo(const _vectorf _vDirection)
{
	Set_State(TRANSFORM::LOOK, _vDirection);
}

shared_ptr<CTransform> CTransform::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	return make_private_shared(CTransform, _pDevice, _pContext);
}

shared_ptr<CComponent> CTransform::Clone(any)
{
	return make_private_shared_copy(CTransform, *this);
}
