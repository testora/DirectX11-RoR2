#include "EnginePCH.h"
#include "Effect.h"
#include "VIBufferInstance.h"

CEffect::CEffect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CEffect::CEffect(const CEffect& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
	{
		MSG_RETURN(E_FAIL, "CEffect::Render", "Failed to CGameObject::Initialize_Prototype");
	}
	
	return S_OK;
}

HRESULT CEffect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffect::Render", "Failed to CGameObject::Initialize");
	}

	return S_OK;
}

void CEffect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_pVIBufferInstance->Update([&](void* _pData, _uint _iNumInstance) { Update_Instance(_pData, _iNumInstance, _fTimeDelta); });
}

void CEffect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CEffect::Fetch(any _arg)
{
	m_pVIBufferInstance->Update([&](void* _pData, _uint _iNumInstance) { Fetch_Instance(_pData, _iNumInstance, _arg); });

	return S_OK;
}

HRESULT CEffect::Render(_uint _iPassIndex)
{
	if (FAILED(__super::Render(_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CEffect::Render", "Failed to CGameObject::Render");
	}

	return S_OK;
}

HRESULT CEffect::Add_Component(const COMPONENT _eComponent)
{
	if (FAILED(__super::Add_Component(_eComponent)))
	{
		MSG_RETURN(E_FAIL, "CEffect::Add_Component", "Failed to CGameObject::Add_Component");
	}

	switch (_eComponent)
	{
	case COMPONENT::VIBUFFER_INSTANCE_POINT:
	case COMPONENT::VIBUFFER_INSTANCE_RECT:
		m_pVIBufferInstance	= Get_Component<CVIBufferInstance>(_eComponent);
		break;
	}

	return S_OK;
}
