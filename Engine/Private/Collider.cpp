#include "EnginePCH.h"
#include "Collider.h"
#include "Collision_Manager.h"
#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "GameObject.h"
#include "Transform.h"
#include "PipeLine.h"

_uint CCollider::s_iColID	= 0;

CCollider::CCollider(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::COLLIDER)
	, m_iColID(s_iColID++)
{
}

CCollider::CCollider(const CCollider& _rhs)
	: CComponent		(_rhs)
#ifdef _DEBUG
	, m_pBatch			(_rhs.m_pBatch)
	, m_pEffect			(_rhs.m_pEffect)
	, m_pInputLayout	(_rhs.m_pInputLayout)
#endif
	, m_iColID			(s_iColID++)
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

HRESULT CCollider::Initialize(shared_ptr<CGameObject> _pObject, COLLIDERDESC _tColliderDesc)
{
	m_pOwner		= _pObject;
	m_pTransform	= _pObject->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_tColliderDesc = _tColliderDesc;

	if (m_pTransform.expired())
	{
		MSG_RETURN(E_FAIL, "CCollider::Initialize", "Failed to Get_Component: TRANSFORM");
	}

	switch (_tColliderDesc.eType)
	{
	case COLLIDER::SPHERE:
		m_pBounding = CBounding_Sphere::Create(&_tColliderDesc);
		break;

	case COLLIDER::AABB:
		m_pBounding = CBounding_AABB::Create(&_tColliderDesc);
		break;

	case COLLIDER::OBB:
		m_pBounding = CBounding_OBB::Create(&_tColliderDesc);
		break;
	}

	if (nullptr == m_pBounding)
	{
		MSG_RETURN(E_FAIL, "CCollider::Initialize", "Failed to Create Bounding");
	}

	CCollision_Manager::Get_Instance()->Register_Collider(_tColliderDesc.eGroup, _pObject, dynamic_pointer_cast<CCollider>(shared_from_this()));

	return S_OK;
}

void CCollider::Tick(_float _fTimeDelta)
{
	m_pBounding->Tick_Transformation(m_pTransform.lock()->Get_Matrix());
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

	m_pBounding->Render(m_pBatch, !m_iColCnt ? _color(1.f, 1.f, 1.f, 1.f) : _color(1.f, 0.f, 0.f, 1.f));

	m_pBatch->End();

	return S_OK;
}
#endif

_bool CCollider::Intersect(shared_ptr<CCollider> _pTargetCollider)
{
	switch (_pTargetCollider->m_tColliderDesc.eType)
	{
		case COLLIDER::SPHERE:
			return m_pBounding->Intersect(*reinterpret_cast<BoundingSphere*>(_pTargetCollider->m_pBounding->Get_Bounding()));
		case COLLIDER::AABB:
			return m_pBounding->Intersect(*reinterpret_cast<BoundingBox*>(_pTargetCollider->m_pBounding->Get_Bounding()));
		case COLLIDER::OBB:
			return m_pBounding->Intersect(*reinterpret_cast<BoundingOrientedBox*>(_pTargetCollider->m_pBounding->Get_Bounding()));
	}

	MSG_RETURN(false, "CCollider::Intersect", "Invalid Type");
}

void CCollider::OnCollisionEnter(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pOther, _float _fTimeDelta)
{
	++m_iColCnt;
	m_pOwner.lock()->OnCollisionEnter(_eGroup, _pOther, _fTimeDelta);
}

void CCollider::OnCollision(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pOther, _float _fTimeDelta)
{
	m_pOwner.lock()->OnCollision(_eGroup, _pOther, _fTimeDelta);
}

void CCollider::OnCollisionExit(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pOther, _float _fTimeDelta)
{
	--m_iColCnt;
	m_pOwner.lock()->OnCollisionExit(_eGroup, _pOther, _fTimeDelta);
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

shared_ptr<CComponent> CCollider::Clone(any _pairObject_Desc)
{
	if (_pairObject_Desc.has_value())
	{
		shared_ptr<CCollider> pInstance = make_private_shared_copy(CCollider, *this);

		pair<shared_ptr<CGameObject>, COLLIDERDESC> pairObjectCollider = any_cast<pair<shared_ptr<CGameObject>, COLLIDERDESC>>(_pairObject_Desc);

		if (FAILED(pInstance->Initialize(pairObjectCollider.first, pairObjectCollider.second)))
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
