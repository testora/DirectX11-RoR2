#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CPipeLine final : public CSingleton<CPipeLine>
{
private:
	explicit CPipeLine() DEFAULT;
	virtual ~CPipeLine() DEFAULT;

public:
	_bool						Is_InClipSpace(_float3 vClipPosition);
	_bool						Is_InClipSpace(_float4x4 mWorld);

	_float4						Get_Transform(const TRANSFORM eState)								{ return Get_Transform(PIPELINE::WORLD).row(IDX(eState)); }
	_float4x4					Get_Transform(const PIPELINE eState)								{ return m_mTransform[IDX(eState)]; }
	void						Set_Transform(const PIPELINE eState, const _float4x4 mTransform)	{ m_mTransform[IDX(eState)] = mTransform; }

	template<typename T>
	shared_ptr<T>				Get_Camera()														{ return dynamic_pointer_cast<T>(m_pCamera); }
	shared_ptr<class CCamera>	Get_Camera()														{ return m_pCamera; }
	void						Set_Camera(shared_ptr<class CCamera> _pCamera)						{ m_pCamera = _pCamera; }

public:
	_float3						To_ClipSpace(_float4x4 mWorld);

#if WIP_FRUSTRUM_CULLING
	_bool						Is_InScreen(const BoundingBox tBoundingBox)							{ return m_tFrustum.Intersects(tBoundingBox); }
	void						Update_Frustum()													{ BoundingFrustum::CreateFromMatrix(m_tFrustum, m_mTransform[IDX(PIPELINE::PROJECTION)]); }
#endif

private:
	_float4x4					m_mTransform[IDX(PIPELINE::MAX)];
	shared_ptr<class CCamera>	m_pCamera;

#if WIP_FRUSTRUM_CULLING
	BoundingFrustum				m_tFrustum;
#endif

	friend CSingleton<CPipeLine>;
};

END
