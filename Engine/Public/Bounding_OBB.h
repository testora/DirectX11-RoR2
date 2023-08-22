#pragma once
#include "Engine_Define.h"
#include "Bounding.h"

BEGIN(Engine)

class ENGINE_DLL CBounding_OBB final : public CBounding
{
private:
	explicit CBounding_OBB() DEFAULT;
	virtual ~CBounding_OBB() DEFAULT;

public:
	virtual HRESULT						Initialize(any pColliderDesc) override;
#ifdef _DEBUG
	virtual HRESULT						Render(shared_ptr<PrimitiveBatch<VertexPositionColor>>) override;
#endif

public:
	virtual void						Tick_Transformation(_matrixf) override;

private:
	BoundingOrientedBox					m_tStaticOBB{};
	BoundingOrientedBox					m_tOBB{};

public:
	static shared_ptr<CBounding_OBB>	Create(any pColliderDesc);
};

END
