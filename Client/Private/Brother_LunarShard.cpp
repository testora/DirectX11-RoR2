#include "ClientPCH.h"
#include "Brother_LunarShard.h"
#include "GameInstance.h"

CBrother_LunarShard::CBrother_LunarShard(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CBrother_LunarShard::CBrother_LunarShard(const CBrother_LunarShard& _rhs)
	: CGameObject(_rhs)
{
}

HRESULT CBrother_LunarShard::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::MODEL);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS);

	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXMESH, g_aNull);
	m_umapComponentArg[COMPONENT::MODEL]	= make_pair(PROTOTYPE_COMPONENT_MODEL_BROTHER_LUNARSHARD, g_aNull);

	m_tEntityDesc.fForwardSpeed				= 150.f;
	m_tEntityDesc.fBackwardSpeed			= 150.f;
	m_tEntityDesc.fLeftSpeed				= 150.f;
	m_tEntityDesc.fRightSpeed				= 150.f;
	m_tEntityDesc.fSpritPower				= 1.f;
	m_tEntityDesc.fJumpPower				= 0.f;

	m_tEntityDesc.vMaxSpeed					= _float3(200.f, 100.f, 200.f);
	m_tEntityDesc.vResist					= _float3(0.95f, 1.f, 0.95f);

	return S_OK;
}

HRESULT CBrother_LunarShard::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Initialize", "Failed to __super::Initialize");
	}

	m_pTargetTransform = Function::Find_Player()->Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Initialize", "Failed to Find_Player:TRANSFORM");
	}

	m_pPhysics->Set_Gravity(false);

	return S_OK;
}

void CBrother_LunarShard::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_fTimeAcc += _fTimeDelta;

	if (5.f < m_fTimeAcc)
	{
		m_pPool->Push(shared_from_gameobject());
	}
	else
	{
		if (5.f > Function::Distance(m_pTransform, m_pTargetTransform))
		{
			m_pPool->Push(shared_from_gameobject());
		}
	}
}

void CBrother_LunarShard::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);
	
	Add_RenderObject(RENDER_GROUP::NONBLEND);
}

HRESULT CBrother_LunarShard::Render()
{
	if (FAILED(__super::Render(0)))
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CBrother_LunarShard::Fetch(any _aPoolPosition)
{
	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Fetch", "Failed to __super::Fetch");
	}

	pair<shared_ptr<CObjectPool>, _float3> pairPoolPosition = any_cast<pair<shared_ptr<CObjectPool>, _float3>>(_aPoolPosition);
	m_pPool = pairPoolPosition.first;

	m_pTransform->Set_State(TRANSFORM::POSITION, pairPoolPosition.second);
	m_pTransform->LookAt(m_pTargetTransform->Get_State(TRANSFORM::POSITION) + _float3(0.f, 1.f, 0.f));
	m_pPhysics->Force(TRANSFORM::LOOK, 50.f);

	m_fTimeAcc = 0.f;

	CGameInstance::Get_Instance()->Play_Sound(Function::Random({ TEXT("brother_shard1"),TEXT("brother_shard2") }), SOUND_CHANNEL::MONSTER_WEAPON,
		m_pTransform->Get_State(TRANSFORM::POSITION), m_pTargetTransform->Get_State(TRANSFORM::POSITION));

	return S_OK;
}

HRESULT CBrother_LunarShard::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Ready_Components", "Failed to Get_Component");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Ready_Components", "Failed to Get_Component");
	}

	return S_OK;
}

HRESULT CBrother_LunarShard::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_pPhysics = Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CBrother_LunarShard::Ready_Behaviors", "Failed to Get_Behavior");
	}

	return S_OK;
}

shared_ptr<CBrother_LunarShard> CBrother_LunarShard::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CBrother_LunarShard> pInstance = make_private_shared(CBrother_LunarShard, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CBrother_LunarShard::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBrother_LunarShard::Clone(any)
{
	shared_ptr<CBrother_LunarShard> pInstance = make_private_shared_copy(CBrother_LunarShard, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CBrother_LunarShard::Clone", "Failed to Initialize");
	}

	return pInstance;
}
