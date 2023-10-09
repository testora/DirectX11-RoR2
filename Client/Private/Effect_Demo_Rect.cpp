#include "ClientPCH.h"
#include "Effect_Demo_Rect.h"
#include "GameInstance.h"

CEffect_Demo_Rect::CEffect_Demo_Rect(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffect(_pDevice, _pContext)
{
}

CEffect_Demo_Rect::CEffect_Demo_Rect(const CEffect_Demo_Rect& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CEffect_Demo_Rect::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::TEXTURE)	| BIT(COMPONENT::VIBUFFER_INSTANCE_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE]					= make_pair(PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT, g_aNull);

	return S_OK;
}

HRESULT CEffect_Demo_Rect::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffect_Demo_Rect::Initialize", "Failed to __super::Initialize");
	}

	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, ARENA_CENTER + _float3(0.f, 0.f, 0.f));

	return S_OK;
}

void CEffect_Demo_Rect::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fTimeAcc += _fTimeDelta;
}

void CEffect_Demo_Rect::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
	
	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CEffect_Demo_Rect::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Demo_Rect::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CEffect_Demo_Rect::Fetch(any _arg)
{
	if (FAILED(__super::Fetch(_arg)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Demo_Rect::Fetch", "Failed to __super::Fetch");
	}

	return S_OK;
}

_bool CEffect_Demo_Rect::Return()
{
	return false;
}

void CEffect_Demo_Rect::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
	
}

void CEffect_Demo_Rect::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
	
}

shared_ptr<CEffect_Demo_Rect> CEffect_Demo_Rect::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffect_Demo_Rect> pInstance = make_private_shared(CEffect_Demo_Rect, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffect_Demo_Rect::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffect_Demo_Rect::Clone(any)
{
	shared_ptr<CEffect_Demo_Rect> pInstance = make_private_shared_copy(CEffect_Demo_Rect, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffect_Demo_Rect::Clone", "Failed to Initialize");
	}

	return pInstance;
}
