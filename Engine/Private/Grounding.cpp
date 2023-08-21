#include "EnginePCH.h"
#include "Grounding.h"
#include "Grid_Manager.h"
#include "ObjectLayer.h"
#include "GameObject.h"
#include "Transform.h"
#include "VIBuffer.h"

CGrounding::CGrounding(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CBehavior(_pDevice, _pContext, BEHAVIOR::GROUNDING)
{
}

CGrounding::CGrounding(const CGrounding& _rhs)
	: CBehavior(_rhs)
{
}

HRESULT CGrounding::Initialize(shared_ptr<class CGameObject> _pOwner)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CGrounding::Initialize", "Failed to __super::Initialize");
	}

	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CGrounding::Initialize", "weak_ptr Expired: m_pOwner");
	}

	shared_ptr<CTransform> pOwnerTransform = m_pOwner.lock()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == pOwnerTransform)
	{
		MSG_RETURN(E_FAIL, "CControl::Initialize", "Failed to Get_Component");
	}

	m_pOwnerTransform = pOwnerTransform;

	return S_OK;
}

void CGrounding::Tick(_float _fTimeDelta)
{
	_float3 vPosition = Intersect_Terrain();
	if (vPosition == m_pOwnerTransform->Get_State(TRANSFORM::POSITION) + _float3(0.f, 2.f, 0.f))
	{
		return;
	}

	m_pOwnerTransform->Set_State(TRANSFORM::POSITION, _float4(vPosition, 1.f));
}

void CGrounding::Late_Tick(_float _fTimeDelta)
{
}

_float3 CGrounding::Intersect_Terrain()
{
	return CGrid_Manager::Get_Instance()->Raycast(m_pOwnerTransform->Get_State(TRANSFORM::POSITION) + _float3(0.f, 2.f, 0.f), _float3(0.f, -1.f, 0.f));
}

shared_ptr<CGrounding> CGrounding::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, shared_ptr<class CGameObject> _pOwner)
{
	shared_ptr<CGrounding> pInstance = make_private_shared(CGrounding, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pOwner)))
	{
		MSG_RETURN(nullptr, "CGrounding::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CGrounding::Clone(any)
{
	return make_private_shared_copy(CGrounding, *this);
}
