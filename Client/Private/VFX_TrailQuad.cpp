#include "ClientPCH.h"
#include "VFX_TrailQuad.h"
#include "GameInstance.h"

CVFX_TrailQuad::CVFX_TrailQuad(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffect(_pDevice, _pContext)
{
}

CVFX_TrailQuad::CVFX_TrailQuad(const CVFX_TrailQuad& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CVFX_TrailQuad::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::TEXTURE)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINT, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE]					= make_pair(PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);

	return S_OK;
}

HRESULT CVFX_TrailQuad::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Initialize", "Failed to __super::Initialize");
	}

	shared_ptr<CTransform> pTransform = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	m_deqDown.resize(11, pTransform->Get_State(TRANSFORM::POSITION));
	m_deqUp.resize(11, pTransform->Get_State(TRANSFORM::POSITION) + pTransform->Get_State(TRANSFORM::UP));

	return S_OK;
}

void CVFX_TrailQuad::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	shared_ptr<CTransform> pTransform = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);

	++m_iIndex;
	if (m_iIndex >= 10)
	{
		m_iIndex = 0;
	}

	m_deqDown.push_front(pTransform->Get_State(TRANSFORM::POSITION));
	m_deqUp.push_front(pTransform->Get_State(TRANSFORM::POSITION) + pTransform->Get_State(TRANSFORM::UP) * 0.1f);
	if (m_deqDown.size() > 10)
	{
		m_deqDown.pop_back();
	}
	if (m_deqUp.size() > 10)
	{
		m_deqUp.pop_back();
	}
}

void CVFX_TrailQuad::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CVFX_TrailQuad::Render()
{
	Get_Component<CShader>(COMPONENT::SHADER)->Bind_Vector(SHADER_MTRLDIF, _float4(0.2f, 0.6f, 0.8f, 1.f));

	if (FAILED(__super::Render(3)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CVFX_TrailQuad::Fetch(any _arg)
{
	if (FAILED(__super::Fetch(_arg)))
	{
		MSG_RETURN(E_FAIL, "CVFX_TrailQuad::Fetch", "Failed to __super::Fetch");
	}

	return S_OK;
}

_bool CVFX_TrailQuad::Return()
{
	return false;
}

void CVFX_TrailQuad::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	VTXINSTTRANSCOLOR* pData = reinterpret_cast<VTXINSTTRANSCOLOR*>(_pData);
}

void CVFX_TrailQuad::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	VTXINSTTRANSCOLOR* pData = reinterpret_cast<VTXINSTTRANSCOLOR*>(_pData);

	pData[m_iIndex].vColor		= _color(1.f, 1.f, 1.f, 0.f);

	for (_uint i = 0; i < _iNumInstance; ++i)
	{
		pData[i].vRight			= _float4(m_deqUp[i], 1.f);
		pData[i].vUp			= _float4(m_deqUp[i + 1], 1.f);
		pData[i].vLook			= _float4(m_deqDown[i + 1], 1.f);
		pData[i].vTranslation	= _float4(m_deqDown[i], 1.f);

		pData[i].vColor.w		= 1.f - (i / static_cast<_float>(_iNumInstance));
	}
}

shared_ptr<CVFX_TrailQuad> CVFX_TrailQuad::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CVFX_TrailQuad> pInstance = make_private_shared(CVFX_TrailQuad, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CVFX_TrailQuad::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CVFX_TrailQuad::Clone(any)
{
	shared_ptr<CVFX_TrailQuad> pInstance = make_private_shared_copy(CVFX_TrailQuad, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CVFX_TrailQuad::Clone", "Failed to Initialize");
	}

	return pInstance;
}
