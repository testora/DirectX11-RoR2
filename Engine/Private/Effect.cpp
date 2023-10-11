#include "EnginePCH.h"
#include "Effect.h"
#include "Component_Manager.h"

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

HRESULT CEffect::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CEffect::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CEffect::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = dynamic_pointer_cast<CShader>(m_umapComponent[COMPONENT::SHADER]);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CEffect::Ready_Components", "Nullptr Exception: m_pShader");
	}

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

HRESULT CEffect::Add_Component(const COMPONENT _eComponent, shared_ptr<CComponent> _pComponent)
{
	if (FAILED(__super::Add_Component(_eComponent, _pComponent)))
	{
		MSG_RETURN(E_FAIL, "CEffect::Add_Component", "Failed to CGameObject::Add_Component");
	}

	switch (_eComponent)
	{
	case COMPONENT::VIBUFFER_INSTANCE_POINT:
	case COMPONENT::VIBUFFER_INSTANCE_LINE:
	case COMPONENT::VIBUFFER_INSTANCE_RECT:
		m_pVIBufferInstance	= Get_Component<CVIBufferInstance>(_eComponent);
		break;
	}

	return S_OK;
}
