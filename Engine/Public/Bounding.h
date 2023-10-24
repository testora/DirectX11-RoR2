#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL IBounding abstract
{
protected:
	explicit IBounding() DEFAULT;
	virtual ~IBounding() DEFAULT;

public:
	virtual HRESULT			Initialize(any pColliderDesc)																				PURE;
#ifdef _DEBUG
	virtual HRESULT			Render(shared_ptr<PrimitiveBatch<VertexPositionColor>>, _vectorf vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f))	PURE;
#endif

public:
	inline void*			Get_Bounding() const	{ return m_pBounding; }

public:
	virtual void			Tick_Transformation(_matrixf)																				PURE;

	virtual _bool			Intersect(BoundingSphere)																					PURE;
	virtual _bool			Intersect(BoundingBox)																						PURE;
	virtual _bool			Intersect(BoundingOrientedBox)																				PURE;

protected:
	const COLLIDERDESC*		m_pColliderDesc	= nullptr;
	void*					m_pBounding		= nullptr;
};

END
