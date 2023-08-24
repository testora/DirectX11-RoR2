#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CPipeLine final : public CSingleton<CPipeLine>
{
private:
	explicit CPipeLine() DEFAULT;
	virtual ~CPipeLine() DEFAULT;

public:
	_float4x4			Get_Transform(const PIPELINE eState)						{ return m_mTransform[IDX(eState)]; }
	void				Set_Transform(const PIPELINE eState, _float4x4 mTransform)	{ m_mTransform[IDX(eState)] = mTransform; }

	_bool				Is_InScreen(const BoundingBox tBoundingBox)					{ return m_tFrustum.Intersects(tBoundingBox); }
	void				Update_Frustum()											{ BoundingFrustum::CreateFromMatrix(m_tFrustum, m_mTransform[IDX(PIPELINE::PROJECTION)]); }

private:
	_float4x4			m_mTransform[IDX(PIPELINE::MAX)];

	BoundingFrustum		m_tFrustum;

	friend CSingleton<CPipeLine>;
};

END
