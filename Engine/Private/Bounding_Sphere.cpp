#include "EnginePCH.h"
#include "Bounding_Sphere.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#endif

HRESULT CBounding_Sphere::Initialize(any _pColliderDesc)
{
	if (FAILED(__super::Initialize(_pColliderDesc)))
	{
		MSG_RETURN(E_FAIL, "CBounding_Sphere::Initialize", "Failed to __super::Initialize");
	}

	m_tStaticSphere = BoundingSphere(m_pColliderDesc->vPosition, m_pColliderDesc->fRadius);
	m_tSphere       = BoundingSphere(m_tStaticSphere);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CBounding_Sphere::Render(shared_ptr<PrimitiveBatch<VertexPositionColor>> _pBatch, _vectorf _vColor)
{
	DX::Draw(_pBatch.get(), m_tSphere, _vColor);

	return S_OK;
}
#endif

void CBounding_Sphere::Tick_Transformation(_matrixf _mWorld)
{
	m_tStaticSphere.Transform(m_tSphere, _mWorld);
}

shared_ptr<CBounding_Sphere> CBounding_Sphere::Create(any _pColliderDesc)
{
	shared_ptr<CBounding_Sphere> pInstance = make_private_shared(CBounding_Sphere);

	if (FAILED(pInstance->Initialize(_pColliderDesc)))
	{
		MSG_RETURN(nullptr, "CBounding_Sphere::Create", "Failed to Initialize");
	}

	return pInstance;
}
