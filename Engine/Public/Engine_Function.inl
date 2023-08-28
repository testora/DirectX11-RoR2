#include "Engine_Function.h"

namespace Function
{
	_float Lerp(_float _fStart, _float _fEnd, _float _fRatio)
	{
		return (_fStart * (1.f - _fRatio)) + (_fEnd * _fRatio);
	}

	XMVECTOR Lerp(FXMVECTOR vStart, FXMVECTOR vEnd, _float fRatio)
	{
		return XMVectorLerp(vStart, vEnd, fRatio);
	}

	XMMATRIX Lerp(FXMMATRIX mStart, CXMMATRIX mEnd, _float fRatio)
	{
		XMVECTOR vStartScale, vStartRotation, vStartTranslation;
		XMVECTOR vEndScale,	vEndRotation, vEndTranslation;

		if (!XMMatrixDecompose(&vStartScale, &vStartRotation, &vStartTranslation, mStart))
		{
			return XMMATRIX{};
		}

		if (!XMMatrixDecompose(&vEndScale, &vEndRotation, &vEndTranslation, mEnd))
		{
			return XMMATRIX{};
		}

		XMVECTOR vScale			= XMVectorLerp(vStartScale, vEndScale, fRatio);
		XMVECTOR vRotation		= XMQuaternionSlerp(vStartRotation, vEndRotation, fRatio);
		XMVECTOR vTranslation	= XMVectorLerp(vStartTranslation, vEndTranslation, fRatio);

		return XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
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

	_bool NearZero2(XMVECTOR _fVector2)
	{
		return XMVector2NearEqual(XMVectorZero(), _fVector2, XMVectorSet(g_fTolorance, g_fTolorance, 0.f, 0.f));
	}

	_bool NearZero3(XMVECTOR _fVector3)
	{
		return XMVector3NearEqual(XMVectorZero(), _fVector3, XMVectorSet(g_fTolorance, g_fTolorance, g_fTolorance, 0.f));
	}

	_bool NearZero4(XMVECTOR _fVector4)
	{
		return XMVector4NearEqual(XMVectorZero(), _fVector4, XMVectorSet(g_fTolorance, g_fTolorance, g_fTolorance, g_fTolorance));
	}

	XMVECTOR QuaternionToEuler(XMVECTOR _vQuaternion, _bool _bToRadians)
	{
		XMFLOAT4 vQuaternion;
		XMStoreFloat4(&vQuaternion, XMQuaternionNormalize(_vQuaternion));

		_float fSquareX = vQuaternion.x * vQuaternion.x;
		_float fSquareY	= vQuaternion.y * vQuaternion.y;
		_float fSquareZ	= vQuaternion.z * vQuaternion.z;
		_float fSquareW	= vQuaternion.w * vQuaternion.w;

		_float fPitch	= asinf(2.f * (vQuaternion.w * vQuaternion.x - vQuaternion.y * vQuaternion.z));
		_float fYaw		= atan2f(2.f * (vQuaternion.x * vQuaternion.z + vQuaternion.w * vQuaternion.y), (-fSquareX - fSquareY + fSquareZ + fSquareW));
		_float fRoll	= atan2f(2.f * (vQuaternion.x * vQuaternion.y + vQuaternion.w * vQuaternion.z), (-fSquareX + fSquareY - fSquareZ + fSquareW));

		return _bToRadians ? XMVectorSet(fPitch, fYaw, fRoll, 0.f) : RadiansToDegrees(XMVectorSet(fPitch, fYaw, fRoll, 0.f));
	}

	XMVECTOR DegreesToRadians(XMVECTOR _vDegrees)
	{
		return _vDegrees * (XM_PI / 180.0f);
	}

	XMVECTOR RadiansToDegrees(XMVECTOR _vRadians)
	{
		return _vRadians * (180.0f / XM_PI);
	}

	string ToString(const wstring& _wstr)
	{
		_uint	strSize	= WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), -1, NULL, 0, NULL, NULL);
		_char*	str		= new _char[strSize];

		WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), -1, str, strSize, NULL, NULL);
		string strResult(str);
		delete[] str;

		return strResult;
	}

	wstring ToWString(const string& _str)
	{
		_uint	wstrSize	= MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, NULL, 0);
		_wchar*	wstr		= new _wchar[wstrSize];

		MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, wstr, wstrSize);
		wstring wstrResult(wstr);
		delete[] wstr;

		return wstrResult;
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
