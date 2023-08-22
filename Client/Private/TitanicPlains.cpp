#include "ClientPCH.h"
#include "TitanicPlains.h"
#include "GameInstance.h"

CTitanicPlains::CTitanicPlains(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTitanicPlains::CTitanicPlains(const CTitanicPlains& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTitanicPlains::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESH, any());
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_TITANICPLAINS, any());

	return S_OK;
}

HRESULT CTitanicPlains::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTitanicPlains::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform->Set_Scale(_float3(0.001f, 0.001f, 0.001f));

	return S_OK;
}

void CTitanicPlains::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTitanicPlains::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
}

HRESULT CTitanicPlains::Render(_uint)
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTitanicPlains::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CTitanicPlains::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTitanicPlains::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CTitanicPlains::Ready_Components", "Nullptr Exception: m_pRenderer");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CTitanicPlains::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	return S_OK;
}

shared_ptr<CTitanicPlains> CTitanicPlains::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTitanicPlains> pInstance = make_private_shared(CTitanicPlains, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTitanicPlains::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTitanicPlains::Clone(any)
{
	shared_ptr<CTitanicPlains> pInstance = make_private_shared_copy(CTitanicPlains, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CTitanicPlains::Create", "Failed to Initialize");
	}

	return pInstance;
}
