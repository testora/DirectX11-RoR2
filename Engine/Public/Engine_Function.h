#pragma once

namespace Function
{
	inline _float		Lerp(_float fStart, _float fEnd, _float fRatio);
	inline _float		Clamp(_float fValue, _float fMin, _float fMax);

	inline _bool		NearZero(_float _fValue);
}

namespace DirectX
{
	inline XMVECTOR		QuaternionBetweenAxis(FXMVECTOR, FXMVECTOR);
}

template <typename T>
void Safe_Delete(T& _p)
{
	if (nullptr != _p)
	{
		delete _p;
		_p = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& _p)
{
	if (nullptr != _p)
	{
		delete[] _p;
		_p = nullptr;
	}
}

#include "Engine_Function.inl"
