#include "EnginePCH.h"
#include "Collider.h"
#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "PipeLine.h"

CCollider::CCollider(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::COLLIDER)
{
}

CCollider::CCollider(const CCollider& _rhs)
	: CComponent		(_rhs)
#ifdef _DEBUG
	, m_pBatch			(_rhs.m_pBatch)
	, m_pEffect			(_rhs.m_pEffect)
	, m_pInputLayout	(_rhs.m_pInputLayout)
#endif
{
}

HRESULT CCollider::Initialize_Prototype()
{
#ifdef _DEBUG
	m_pEffect	= make_shared<BasicEffect>(m_pDevice.Get());
	m_pBatch	= make_shared<PrimitiveBatch<VertexPositionColor>>(m_pContext.Get());

	const void*	pShaderByteCode			= nullptr;
	size_t		nShaderByteCodeLength	= 0;
	m_pEffect->SetVertexColorEnabled(true);
	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &nShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, nShaderByteCodeLength, m_pInputLayout.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CCollider::Initialize_Prototype", "Failed to CreateInputLayout");
	}
#endif

	return S_OK;
}

HRESULT CCollider::Initialize(COLLIDERDESC _tColliderDesc)
{
	switch (m_tColliderDesc.eType)
	{
	case COLLIDER::SPHERE:
		m_pBounding = CBounding_Sphere::Create(&m_tColliderDesc);
		break;

	case COLLIDER::AABB:
		m_pBounding = CBounding_AABB::Create(&m_tColliderDesc);
		break;

	case COLLIDER::OBB:
		m_pBounding = CBounding_OBB::Create(&m_tColliderDesc);
		break;
	}

	if (nullptr == m_pBounding)
	{
		MSG_RETURN(E_FAIL, "CCollider::Initialize", "Failed to Create Bounding");
	}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CCollider::Render()
{
	m_pContext->IASetInputLayout(m_pInputLayout.Get());

	m_pEffect->Apply(m_pContext.Get());
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW));
	m_pEffect->SetProjection(CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION));

	m_pBatch->Begin();

	m_pBounding->Render(m_pBatch);

	m_pBatch->End();

	return S_OK;
}
#endif

void CCollider::Tick_Transformation(_matrixf _mWorld)
{
	m_pBounding->Tick_Transformation(_mWorld);
}

shared_ptr<CCollider> CCollider::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCollider> pInstance = make_private_shared(CCollider, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CCollider::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CCollider::Clone(any _tColliderDesc)
{
	if (_tColliderDesc.has_value())
	{
		shared_ptr<CCollider> pInstance = make_private_shared_copy(CCollider, *this);

		COLLIDERDESC tDesc = any_cast<COLLIDERDESC>(_tColliderDesc);

		if (FAILED(pInstance->Initialize(tDesc)))
		{
			MSG_RETURN(nullptr, "CCollider::Create", "Failed to Initialize");
		}

		return pInstance;
	}
	else
	{
		MSG_RETURN(nullptr, "CCollider::Clone", "Invalid Parameter");
	}
}
