#include "ClientPCH.h"
#include "Effect_Demo_Point.h"
#include "GameInstance.h"

CEffect_Demo_Point::CEffect_Demo_Point(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CEffect(_pDevice, _pContext)
{
}

CEffect_Demo_Point::CEffect_Demo_Point(const CEffect_Demo_Point& _rhs)
	: CEffect(_rhs)
{
}

HRESULT CEffect_Demo_Point::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::TEXTURE)	| BIT(COMPONENT::VIBUFFER_INSTANCE_POINT);

	m_umapComponentArg[COMPONENT::RENDERER]					= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]					= make_pair(PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINT, g_aNull);
	m_umapComponentArg[COMPONENT::TEXTURE]					= make_pair(PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_INSTANCE_POINT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT, g_aNull);

	return S_OK;
}

HRESULT CEffect_Demo_Point::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CEffect_Demo_Point::Initialize", "Failed to __super::Initialize");
	}

	Get_Component<CTransform>(COMPONENT::TRANSFORM)->Set_State(TRANSFORM::POSITION, ARENA_CENTER + _float3(0.f, 0.f, 0.f));

	return S_OK;
}

void CEffect_Demo_Point::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fTimeAcc += _fTimeDelta;
}

void CEffect_Demo_Point::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CEffect_Demo_Point::Render()
{
	if (FAILED(__super::Render(1)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Demo_Point::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CEffect_Demo_Point::Fetch(any _arg)
{
	if (FAILED(__super::Fetch(_arg)))
	{
		MSG_RETURN(E_FAIL, "CEffect_Demo_Point::Fetch", "Failed to __super::Fetch");
	}

	return S_OK;
}

_bool CEffect_Demo_Point::Return()
{
	if (3.f < m_fTimeAcc)
	{
		return true;
	}

	return false;
}

void CEffect_Demo_Point::Fetch_Instance(void* _pData, _uint _iNumInstance, any _arg)
{
//	_float4 vPosition = any_cast<_float4>(_arg);
//
//	VTXPOSSIZEINSTTRANS* pData = reinterpret_cast<VTXPOSSIZEINSTTRANS*>(_pData);
//
//	for (_uint i = 0; i < _iNumInstance; ++i)
//	{
//		pData[i].vTranslation = vPosition;
//	}
//
//	m_fTimeAcc = 0.f;
}

void CEffect_Demo_Point::Update_Instance(void* _pData, _uint _iNumInstance, _float _fTimeDelta)
{
//	VTXINSTPOINT* pData = reinterpret_cast<VTXINSTPOINT*>(_pData);
//
//	for (_uint i = 0; i < _iNumInstance; ++i)
//	{
//		i ? pData[i].vTranslation.x += _fTimeDelta : pData[i].vTranslation.x -= _fTimeDelta;
//		pData[i].vTranslation.y += _fTimeDelta;
//	}
}

shared_ptr<CEffect_Demo_Point> CEffect_Demo_Point::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CEffect_Demo_Point> pInstance = make_private_shared(CEffect_Demo_Point, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CEffect_Demo_Point::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CEffect_Demo_Point::Clone(any)
{
	shared_ptr<CEffect_Demo_Point> pInstance = make_private_shared_copy(CEffect_Demo_Point, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CEffect_Demo_Point::Clone", "Failed to Initialize");
	}

	return pInstance;
}
