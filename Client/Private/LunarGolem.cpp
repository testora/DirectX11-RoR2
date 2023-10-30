#include "ClientPCH.h"
#include "LunarGolem.h"
#include "GameInstance.h"
#include "LunarGolem_BehaviorTree.h"

CLunarGolem::CLunarGolem(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CLunarGolem::CLunarGolem(const CLunarGolem& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CLunarGolem::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS)	| BIT(BEHAVIOR::GROUNDING)	| BIT(BEHAVIOR::ANIMATOR)	| BIT(BEHAVIOR::CUSTOM);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_LUNARGOLEM, g_aNull);

	m_umapBehaviorArg[BEHAVIOR::GROUNDING]	= make_pair(wstring(), wstring(GRID_TERRAIN));

	m_tEntityDesc.fMaxHP					= 10.f;
	m_tEntityDesc.fHP						= m_tEntityDesc.fMaxHP;

	m_tEntityDesc.fForwardSpeed				= 30.f;
	m_tEntityDesc.fBackwardSpeed			= 30.f;
	m_tEntityDesc.fLeftSpeed				= 30.f;
	m_tEntityDesc.fRightSpeed				= 30.f;
	m_tEntityDesc.fSpritPower				= 5.f;
	m_tEntityDesc.fJumpPower				= 60.f;

	m_tEntityDesc.vMaxSpeed					= _float3(100.f, 200.f, 100.f);
	m_tEntityDesc.vResist					= _float3(0.1f, 0.2f, 0.1f);

	return S_OK;
}

HRESULT CLunarGolem::Initialize(any)
{
	if (FAILED(__super::Initialize("head")))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform->Set_Scale(_float3(2.f, 2.f, 2.f));
	m_pAnimator->Play_Animation(ANIMATION::LUNAR_GOLEM::IDLE);
	m_pAnimator->Set_Default(ANIMATION::LUNAR_GOLEM::IDLE);

	LIGHTDESC tLightDesc{};
	tLightDesc.eLightType	= LIGHTTYPE::SHADOW;
	tLightDesc.eShadowType	= SHADOWTYPE::DIRECTIONAL;
	tLightDesc.fRange		= 10.f;
	tLightDesc.vDirection	= _float3(-0.64f, -0.76f, -0.12f);
	CGameInstance::Get_Instance()->Add_Light(SCENE::MOON, tLightDesc, m_pTransform, shared_from_gameobject());

	return S_OK;
}

void CLunarGolem::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CLunarGolem::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::SHADOW);
	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CLunarGolem::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CLunarGolem::Render_ShadowDepth()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem::Render_ShadowDepth", "Failed to Bind_OnShader");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CLunarGolem::Fetch(any _vPosition3)
{
	if (FAILED(__super::Fetch(_vPosition3)))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem::Fetch", "Failed to __super::Fetch");
	}

	return S_OK;
}

_bool CLunarGolem::Return()
{
	return false;
}

HRESULT CLunarGolem::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem::Ready_Components", "Failed to __super::Ready_Components");
	}

	return S_OK;
}

HRESULT CLunarGolem::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CLunarGolem::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_umapBehavior.emplace(BEHAVIOR::CUSTOM, CLunarGolem_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc));

	return S_OK;
}

void CLunarGolem::Hit()
{
	m_tEntityDesc.fHP -= 1.f;
}

void CLunarGolem::Hit_WeakPoint()
{
	m_tEntityDesc.fHP -= 5.f;
}

shared_ptr<CLunarGolem> CLunarGolem::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CLunarGolem> pInstance = make_private_shared(CLunarGolem, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CLunarGolem::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CLunarGolem::Clone(any)
{
	shared_ptr<CLunarGolem> pInstance = make_private_shared_copy(CLunarGolem, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CLunarGolem::Clone", "Failed to Initialize");
	}

	return pInstance;
}
