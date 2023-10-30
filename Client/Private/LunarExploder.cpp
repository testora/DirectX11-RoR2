#include "ClientPCH.h"
#include "LunarExploder.h"
#include "GameInstance.h"
#include "LunarExploder_BehaviorTree.h"

CLunarExploder::CLunarExploder(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CLunarExploder::CLunarExploder(const CLunarExploder& _rhs)
	: CMonster(_rhs)
{
}

HRESULT CLunarExploder::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS)	| BIT(BEHAVIOR::GROUNDING)	| BIT(BEHAVIOR::ANIMATOR)	| BIT(BEHAVIOR::CUSTOM);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_LUNAREXPLODER, g_aNull);

	m_umapBehaviorArg[BEHAVIOR::GROUNDING]	= make_pair(wstring(), wstring(GRID_TERRAIN));

	m_tEntityDesc.fMaxHP					= 10.f;
	m_tEntityDesc.fHP						= m_tEntityDesc.fMaxHP;

	m_tEntityDesc.fForwardSpeed				= 60.f;
	m_tEntityDesc.fBackwardSpeed			= 60.f;
	m_tEntityDesc.fLeftSpeed				= 60.f;
	m_tEntityDesc.fRightSpeed				= 60.f;
	m_tEntityDesc.fSpritPower				= 1.f;
	m_tEntityDesc.fJumpPower				= BROTHER_JUMP_POWER;

	m_tEntityDesc.vMaxSpeed					= _float3(50.f, 200.f, 50.f);
	m_tEntityDesc.vResist					= _float3(0.05f, 0.2f, 0.05f);

	return S_OK;
}

HRESULT CLunarExploder::Initialize(any)
{
	if (FAILED(__super::Initialize("core")))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder::Initialize", "Failed to __super::Initialize");
	}

	Get_Behavior<CAnimator>(BEHAVIOR::ANIMATOR)->Play_Animation(ANIMATION::LUNAR_EXPLODER::IDLE);

	LIGHTDESC tLightDesc{};
	tLightDesc.eLightType	= LIGHTTYPE::SHADOW;
	tLightDesc.eShadowType	= SHADOWTYPE::DIRECTIONAL;
	tLightDesc.fRange		= 10.f;
	tLightDesc.vDirection	= _float3(-0.64f, -0.76f, -0.12f);
	CGameInstance::Get_Instance()->Add_Light(SCENE::MOON, tLightDesc, m_pTransform, shared_from_gameobject());

	return S_OK;
}

void CLunarExploder::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CLunarExploder::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::SHADOW);
	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CLunarExploder::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CLunarExploder::Render_ShadowDepth()
{
	if (FAILED(m_pTransform->Bind_OnShader(m_pShader)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder::Render_ShadowDepth", "Failed to Bind_OnShader");
	}

	if (FAILED(m_pModel->Render_ShadowDepth(shared_from_gameobject(), m_pShader, 2)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder::Render_ShadowDepth", "Failed to Render_ShadowDepth");
	}

	return S_OK;
}

HRESULT CLunarExploder::Fetch(any _vPosition3)
{
	if (FAILED(__super::Fetch(_vPosition3)))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder::Fetch", "Failed to __super::Fetch");
	}

	return S_OK;
}

_bool CLunarExploder::Return()
{
	return false;
}

HRESULT CLunarExploder::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder::Ready_Components", "Failed to __super::Ready_Components");
	}

	return S_OK;
}

HRESULT CLunarExploder::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CLunarExploder::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_umapBehavior.emplace(BEHAVIOR::CUSTOM, CLunarExploder_BehaviorTree::Create(shared_from_gameobject(), &m_tEntityDesc));

	return S_OK;
}

void CLunarExploder::Hit()
{
	m_tEntityDesc.fHP -= 1.f;
}

void CLunarExploder::Hit_WeakPoint()
{
	m_tEntityDesc.fHP -= 5.f;
}

shared_ptr<CLunarExploder> CLunarExploder::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CLunarExploder> pInstance = make_private_shared(CLunarExploder, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CLunarExploder::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CLunarExploder::Clone(any)
{
	shared_ptr<CLunarExploder> pInstance = make_private_shared_copy(CLunarExploder, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CLunarExploder::Clone", "Failed to Initialize");
	}

	return pInstance;
}
