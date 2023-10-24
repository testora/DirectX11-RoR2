#pragma once
#include "Engine_Define.h"
#include "Bounding.h"

BEGIN(Engine)

class ENGINE_DLL CBounding_Sphere final : public IBounding
{
private:
	explicit CBounding_Sphere() DEFAULT;
	virtual ~CBounding_Sphere() DEFAULT;

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
	BoundingSphere						m_tStaticSphere{};
	BoundingSphere						m_tSphere{};

public:
	static shared_ptr<CBounding_Sphere>	Create(any pColliderDesc);
};

END
