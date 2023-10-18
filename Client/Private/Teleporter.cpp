#include "ClientPCH.h"
#include "Teleporter.h"
#include "GameInstance.h"

CTeleporter::CTeleporter(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CTeleporter::CTeleporter(const CTeleporter& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CTeleporter::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESH, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL]			= make_pair(PROTOTYPE_COMPONENT_MODEL_TELEPORTER, g_aNull);

	return S_OK;
}

HRESULT CTeleporter::Initialize(any _vPosition3)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CTeleporter::Initialize", "Failed to __super::Initialize");
	}

	if (_vPosition3.has_value())
	{
		Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, any_cast<_float3>(_vPosition3));
	}

	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Rotate(TRANSFORM::UP, XM_PIDIV4);

	return S_OK;
}

void CTeleporter::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CTeleporter::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CTeleporter::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CTeleporter::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CTeleporter::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CTeleporter::Ready_Components", "Failed to __super::Ready_Components");
	}

	return S_OK;
}

shared_ptr<CTeleporter> CTeleporter::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CTeleporter> pInstance = make_private_shared(CTeleporter, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CTeleporter::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CTeleporter::Clone(any _vPosition3)
{
	shared_ptr<CTeleporter> pInstance = make_private_shared_copy(CTeleporter, *this);

	if (FAILED(pInstance->Initialize(_vPosition3)))
	{
		MSG_RETURN(nullptr, "CTeleporter::Clone", "Failed to Initialize");
	}

	return pInstance;
}
