#include "EnginePCH.h"
#include "Bounding_OBB.h"
#ifdef _DEBUG
#include "DebugDraw.h"
#endif

HRESULT CBounding_OBB::Initialize(any _pColliderDesc)
{
	if (FAILED(__super::Initialize(_pColliderDesc)))
	{
		MSG_RETURN(E_FAIL, "CBounding_Sphere::Initialize", "Failed to __super::Initialize");
	}

	m_tStaticOBB	= BoundingOrientedBox(m_pColliderDesc->vPosition, m_pColliderDesc->vSize, _float4(XMQuaternionRotationRollPitchYawFromVector(_float3(m_pColliderDesc->vRotation))));
	m_tOBB			= BoundingOrientedBox(m_tStaticOBB);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CBounding_OBB::Render(shared_ptr<PrimitiveBatch<VertexPositionColor>> _pBatch, _vectorf _vColor)
{
	DX::Draw(_pBatch.get(), m_tOBB, _vColor);

	return S_OK;
}
#endif

void CBounding_OBB::Tick_Transformation(_matrixf _mWorld)
{
	m_tStaticOBB.Transform(m_tOBB, _mWorld);
}

shared_ptr<CBounding_OBB> CBounding_OBB::Create(any _pColliderDesc)
{
	shared_ptr<CBounding_OBB> pInstance = make_private_shared(CBounding_OBB);

	if (FAILED(pInstance->Initialize(_pColliderDesc)))
	{
		MSG_RETURN(nullptr, "CBounding_OBB::Create", "Failed to Initialize");
	}

	return pInstance;
}
