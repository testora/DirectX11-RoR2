#pragma once
#include "Engine_Define.h"
#include "Bounding.h"

BEGIN(Engine)

class ENGINE_DLL CBounding_AABB final : public IBounding
{
private:
	explicit CBounding_AABB() DEFAULT;
	virtual ~CBounding_AABB() DEFAULT;

public:
	virtual HRESULT						Initialize(any pColliderDesc) override;
#ifdef _DEBUG
	virtual HRESULT						Render(shared_ptr<PrimitiveBatch<VertexPositionColor>>, _vectorf vColor) override;
#endif

public:
	virtual void						Tick_Transformation(_matrixf) override;

	inline virtual _bool				Intersect(BoundingSphere) override;
	inline virtual _bool				Intersect(BoundingBox) override;
	inline virtual _bool				Intersect(BoundingOrientedBox) override;

private:
	BoundingBox							m_tStaticAABB{};
	BoundingBox							m_tAABB{};

public:
	static shared_ptr<CBounding_AABB>	Create(any pColliderDesc);
};

END
