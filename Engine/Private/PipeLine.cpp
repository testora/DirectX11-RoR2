#include "EnginePCH.h"
#include "PipeLine.h"

_bool CPipeLine::Is_InClipSpace(_float3 _vClipPosition)
{
	return Function::InRange(_vClipPosition.x, -1.f, 1.f) && Function::InRange(_vClipPosition.y, -1.f, 1.f) && Function::InRange(_vClipPosition.z, 0.f, 1.f);
}

_bool CPipeLine::Is_InClipSpace(_float4x4 _mWorld)
{
	_float3 vClip = To_ClipSpace(_mWorld);

	return Function::InRange(vClip.x, -1.f, 1.f) && Function::InRange(vClip.y, -1.f, 1.f) && Function::InRange(vClip.z, 0.f, 1.f);
}

_float3 CPipeLine::To_ClipSpace(_float4x4 _mWorld)
{
	_matrix mToClipSpace = _mWorld * m_mTransform[IDX(PIPELINE::VIEW)] * m_mTransform[IDX(PIPELINE::PROJECTION)];

	return XMVector3TransformCoord(_float3(), mToClipSpace);
}
