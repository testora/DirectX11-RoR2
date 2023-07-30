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

#include "Engine_Function.inl"
