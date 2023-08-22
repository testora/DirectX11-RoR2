#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBounding abstract
{
protected:
	explicit CBounding() DEFAULT;
	virtual ~CBounding() DEFAULT;

public:
	virtual HRESULT			Initialize(any pColliderDesc)							PURE;
#ifdef _DEBUG
	virtual HRESULT			Render(shared_ptr<PrimitiveBatch<VertexPositionColor>>)	PURE;
#endif

protected:
	const COLLIDERDESC*		m_pColliderDesc	= nullptr;

public:
	virtual void			Tick_Transformation(_matrixf)							PURE;
};

END
