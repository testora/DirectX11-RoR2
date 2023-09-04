#include "ClientPCH.h"
#include "GolemPlains.h"
#include "GameInstance.h"

CGolemPlains::CGolemPlains(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CGolemPlains::CGolemPlains(const CGolemPlains& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CGolemPlains::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);

	MATERIALDESC tTerrainMaterial;
	tTerrainMaterial.vDiffuse = _float4(0.58f, 0.56f, 0.64f, 1.f);

	map<_uint, tuple<MATERIALDESC, _flags, function<HRESULT(shared_ptr<CShader>)>>> mapDesc;
	mapDesc.emplace(0, make_tuple(
		MATERIALDESC(),
		SHADER_FLAG_TRIPLANER_POSITIVE_Y,
		[](shared_ptr<CShader> _pShader)->HRESULT
		{
			if (FAILED(_pShader->Bind_Float(SHADER_TILING_DIFFUSE, 3.f)))
			{
				MSG_RETURN(E_FAIL, "CGolemPlains::Initialize_Prototype", "Failed to Bind_Float");
			}
			if (FAILED(_pShader->Bind_Float(SHADER_TILING_NORMAL, 3.f)))
			{
				MSG_RETURN(E_FAIL, "CGolemPlains::Initialize_Prototype", "Failed to Bind_Float");
			}
			return S_OK;
		})
	);
	mapDesc.emplace(7, make_tuple(
		tTerrainMaterial,
		SHADER_FLAG_TRIPLANER_POSITIVE_Y,
		[](shared_ptr<CShader> _pShader)->HRESULT
		{
			if (FAILED(_pShader->Bind_Float(SHADER_TILING_DIFFUSE, 1.f)))
			{
				MSG_RETURN(E_FAIL, "CGolemPlains::Initialize_Prototype", "Failed to Bind_Float");
			}
			if (FAILED(_pShader->Bind_Float(SHADER_TILING_NORMAL, 1.f)))
			{
				MSG_RETURN(E_FAIL, "CGolemPlains::Initialize_Prototype", "Failed to Bind_Float");
			}
			return S_OK;
		})
	);
	mapDesc.emplace(8, make_tuple(
		tTerrainMaterial,
		SHADER_FLAG_TRIPLANER_POSITIVE_X	| SHADER_FLAG_TRIPLANER_POSITIVE_Y	| SHADER_FLAG_TRIPLANER_POSITIVE_Z	|
		SHADER_FLAG_TRIPLANER_NEGATIVE_X	| SHADER_FLAG_TRIPLANER_NEGATIVE_Y	| SHADER_FLAG_TRIPLANER_NEGATIVE_Z	|
		SHADER_FLAG_TRIPLANER_SHARE_X		| SHADER_FLAG_TRIPLANER_SHARE_Z		| SHADER_FLAG_TRIPLANER_SHARE_X_Z	| SHADER_FLAG_TRIPLANER_SYNC_XZ,
		[](shared_ptr<CShader> _pShader)->HRESULT
		{
			const _float fTilingDif[3]{ 0.05f,	0.05f,	0.05f };
			const _float fTilingNor[3]{ 0.f,	0.f,	1.5f };
			if (FAILED(_pShader->Bind_FloatArray(SHADER_TILING_DIFFUSE, fTilingDif, 3)))
			{
				MSG_RETURN(E_FAIL, "CGolemPlains::Initialize_Prototype", "Failed to Bind_FloatArray");
			}
			if (FAILED(_pShader->Bind_FloatArray(SHADER_TILING_NORMAL, fTilingNor, 3)))
			{
				MSG_RETURN(E_FAIL, "CGolemPlains::Initialize_Prototype", "Failed to Bind_FloatArray");
			}
			return S_OK;
		})
	);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESH, any());
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS, mapDesc);

	return S_OK;
}

HRESULT CGolemPlains::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CGolemPlains::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CGolemPlains::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRenderer->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CGolemPlains::Render()
{
	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Render", "Failed to __super::Render");
	}
	return S_OK;
}

HRESULT CGolemPlains::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Nullptr Exception: m_pRenderer");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CGolemPlains::Ready_Components", "Nullptr Exception: m_pShader");
	}

	return S_OK;
}

shared_ptr<CGolemPlains> CGolemPlains::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CGolemPlains> pInstance = make_private_shared(CGolemPlains, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CGolemPlains::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CGolemPlains::Clone(any)
{
	shared_ptr<CGolemPlains> pInstance = make_private_shared_copy(CGolemPlains, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CGolemPlains::Create", "Failed to Initialize");
	}

	return pInstance;
}
