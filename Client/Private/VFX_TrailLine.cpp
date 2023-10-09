#include "ClientPCH.h"
#include "VFX_TrailLine.h"
#include "GameInstance.h"

CVFX_TrailLine::CVFX_TrailLine(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffect(_pDevice, _pContext)
{
}

CVFX_TrailLine::CVFX_TrailLine(const CVFX_TrailLine& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CVFX_TrailLine::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::TEXTURE)	| BIT(COMPONENT::VIBUFFER_INSTANCE_LINE);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINE, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE]					= make_pair(PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_LINE]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE, g_aNull);

	return S_OK;
}

HRESULT CVFX_TrailLine::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Initialize", "Failed to __super::Initialize");
	}

	shared_ptr<CTransform> pTransform = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_deqLine.resize(13, _float3(0.f, 0.5f, 0.f) + pTransform->Get_State(TRANSFORM::POSITION));

	return S_OK;
}

void CVFX_TrailLine::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	shared_ptr<CTransform> pTransform = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	++m_iIndex;
	if (m_iIndex >= 10)
	{
		m_iIndex = 0;
	}

	m_deqLine.push_front(_float3(0.f, 0.1f, 0.f) + pTransform->Get_State(TRANSFORM::POSITION));
	if (m_deqLine.size() > 12)
	{
		m_deqLine.pop_back();
	}
}

void CVFX_TrailLine::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CVFX_TrailLine::Render()
{
	Get_Component<CShader>(COMPONENT::SHADER)->Bind_Vector(SHADER_MTRLDIF, _float4(0.2f, 0.6f, 0.8f, 1.f));
	Get_Component<CShader>(COMPONENT::SHADER)->Bind_Float(SHADER_THICKNESS, 0.1f);

	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CVFX_TrailLine::Fetch(any _arg)
{
	if (FAILED(__super::Fetch(_arg)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailLine::Fetch", "Failed to __super::Fetch");
	}

	return S_OK;
}

_bool CVFX_TrailLine::Return()
{
	return false;
}

void CVFX_TrailLine::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	VTXINSTTRANSCOLOR* pData = reinterpret_cast<VTXINSTTRANSCOLOR*>(_pData);
}

void CVFX_TrailLine::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXINSTTRANSCOLOR* pData = reinterpret_cast<VTXINSTTRANSCOLOR*>(_pData);

	pData[m_iIndex].vColor		= _color(1.f, 1.f, 1.f, 0.f);

	for (_uint i = 0; i < _iNumInstance; ++i)
	{
		pData[i].vRight			= _float4(m_deqLine[i + 0], 1.f);
		pData[i].vUp			= _float4(m_deqLine[i + 1], 1.f);
		pData[i].vLook			= _float4(m_deqLine[i + 2], 1.f);
		pData[i].vTranslation	= _float4(m_deqLine[i + 3], 1.f);

		pData[i].vColor.w		= 1.f - (i / static_cast<_float>(_iNumInstance));
	}
}

shared_ptr<CVFX_TrailLine> CVFX_TrailLine::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CVFX_TrailLine> pInstance = make_private_shared(CVFX_TrailLine, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CVFX_TrailLine::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CVFX_TrailLine::Clone(any)
{
	shared_ptr<CVFX_TrailLine> pInstance = make_private_shared_copy(CVFX_TrailLine, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CVFX_TrailLine::Clone", "Failed to Initialize");
	}

	return pInstance;
}
