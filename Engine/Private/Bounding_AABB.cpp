#include "EnginePCH.h"
#include "Bounding_AABB.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#endif

HRESULT CBounding_AABB::Initialize(any _pColliderDesc)
{
	if (FAILED(__super::Initialize(_pColliderDesc)))
	{
		MSG_RETURN(E_FAIL, "CBounding_Sphere::Initialize", "Failed to __super::Initialize");
	}

	m_tStaticAABB	= BoundingBox(m_pColliderDesc->vPosition, m_pColliderDesc->vSize);
	m_tAABB			= BoundingBox(m_tStaticAABB);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CBounding_AABB::Render(shared_ptr<PrimitiveBatch<VertexPositionColor>> _pBatch)
{
	DX::Draw(_pBatch.get(), m_tAABB);

	return S_OK;
}
#endif

void CBounding_AABB::Tick_Transformation(_matrixf _mWorld)
{
	m_tStaticAABB.Transform(m_tAABB, _mWorld);
}

shared_ptr<CBounding_AABB> CBounding_AABB::Create(any _pColliderDesc)
{
	shared_ptr<CBounding_AABB> pInstance = make_private_shared(CBounding_AABB);

	if (FAILED(pInstance->Initialize(_pColliderDesc)))
	{
		MSG_RETURN(nullptr, "CBounding_AABB::Create", "Failed to Initialize");
	}

	return pInstance;
}
