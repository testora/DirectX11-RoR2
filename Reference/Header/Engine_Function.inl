#include "Engine_Function.h"
namespace Function
{
	_float Lerp(_float _fStart, _float _fEnd, _float _fRatio)
	{
		return (_fStart * (1.f - _fRatio)) + (_fEnd * _fRatio);
	}

	_float Clamp(_float _fMin, _float _fMax, _float _fValue)
	{
		if (_fValue < _fMin) return _fMin;
		if (_fValue > _fMax) return _fMax;
		return _fValue;
	}

	_float ProportionalRatio(_float _fMin, _float _fMax, _float _fValue)
	{
		if (_fMin == _fMax) return 0.f;
		else return (_fValue - _fMin) / (_fMax - _fMin);
	}

	_bool NearZero(_float _fValue)
	{
		return fabsf(_fValue) <= g_fTolorance;
	}

	_bool NearZero3(XMVECTOR _fVector3)
	{
		return XMVector3NearEqual(XMVectorZero(), _fVector3, XMVectorSet(g_fTolorance, g_fTolorance, g_fTolorance, g_fTolorance));
	}

	_bool NearZero4(XMVECTOR _fVector4)
	{
		return XMVector4NearEqual(XMVectorZero(), _fVector4, XMVectorSet(g_fTolorance, g_fTolorance, g_fTolorance, g_fTolorance));
	}
}

namespace DirectX
{
	XMVECTOR QuaternionBetweenAxis(FXMVECTOR _vAxisA, FXMVECTOR _vAxisB)
	{
		XMVECTOR vAxisA	= XMVector3Normalize(_vAxisA);
		XMVECTOR vAxisB = XMVector3Normalize(_vAxisB);

		XMVECTOR vCross	= XMVector3Cross(vAxisA, vAxisB);
		_float fDot		= XMVectorGetX(XMVector3Dot(vAxisA, vAxisB));

		if (XMVector3Equal(vCross, XMVectorZero()))
		{
			return fDot > 0 ? XMQuaternionIdentity() : XMQuaternionRotationAxis(vAxisA, XM_PI);
		}

		return XMQuaternionRotationAxis(vCross, acosf(fDot));
	}
}
