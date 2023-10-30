#include "ClientPCH.h"
#include "RailGunner_PistolBullet.h"
#include "GameInstance.h"
#include "Monster.h"
#include "Camera_Main.h"
#include "VFX_TrailLine.h"

#define MASK_COLOR1		_color(0.99f, 0.95f, 0.70f, 1.00f)
#define MASK_COLOR2		_color(1.00f, 0.40f, 0.30f, 1.00f)

CRailGunner_PistolBullet::CRailGunner_PistolBullet(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CRailGunner_PistolBullet::CRailGunner_PistolBullet(const CRailGunner_PistolBullet& _rhs)
	: CGameObject	(_rhs)
	, m_pTexMask	(_rhs.m_pTexMask)
{
}

HRESULT CRailGunner_PistolBullet::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_RECT)	| BIT(COMPONENT::COLLIDER);
	m_bitBehavior	|= BIT(BEHAVIOR::PHYSICS);

	COLLIDERDESC tColliderDesc{};
	tColliderDesc.eType		= COLLIDER::SPHERE;
	tColliderDesc.eGroup	= COLLISION_GROUP::PLAYER_BULLET;
	tColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	tColliderDesc.fRadius	= 0.5f;

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::COLLIDER]			= make_pair(PROTOTYPE_COMPONENT_COLLIDER, tColliderDesc);

	m_tEntityDesc.fForwardSpeed	= PISTOLBULLET_SPEED_FORWARD;
	m_tEntityDesc.vMaxSpeed		= PISTOLBULLET_SPEED_TERMINAL;
	m_tEntityDesc.vResist		= PISTOLBULLET_SPEED_RESIST;

	m_pTexMask = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Bullet/texRailgunPistolSquare.png"));

	return S_OK;
}

HRESULT CRailGunner_PistolBullet::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Initialize", "__super::Initialize Failed");
	}

	m_pVFX_Trail_Pool = CGameInstance::Get_Instance()->Find_Pool(CGameInstance::Get_Instance()->Current_Scene(), POOL_EFFECT_TRAIL_LINE);
	if (nullptr == m_pVFX_Trail_Pool)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Initialize", "Find_Pool: POOL_EFFECT_TRAIL_LINE");
	}

	m_pPhysics->Set_Gravity(false);

	return S_OK;
}

void CRailGunner_PistolBullet::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	Manage_State(_fTimeDelta);
	Search_Target();
}

void CRailGunner_PistolBullet::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::BLEND);
}

HRESULT CRailGunner_PistolBullet::Render()
{
	if (FAILED(m_pTexMask->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
	}

	if (FAILED(__super::Render(6)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CRailGunner_PistolBullet::Fetch(any _aPoolPosition)
{
	m_eState			= STATE::UNTARGET;
	m_pTargetTransform	= nullptr;
	m_pTarget			= nullptr;

	if (FAILED(__super::Fetch()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Fetch", "Failed to __super::Fetch");
	}

	pair<shared_ptr<CObjectPool>, _float3> pairPoolPosition = any_cast<pair<shared_ptr<CObjectPool>, _float3>>(_aPoolPosition);
	m_pPool		= pairPoolPosition.first;
	
	m_pTransform->Set_State(TRANSFORM::POSITION, pairPoolPosition.second);
	m_vDirection = _float3(
		CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::RIGHT)	* Function::RandomFloat(-0.15f, 0.15f) +
		CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::UP)		* Function::RandomFloat(-0.05f, 0.05f) +
		CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::LOOK));
	m_pPhysics->Flattern(true, true, true);
	m_pPhysics->Force(m_vDirection, m_tEntityDesc.fForwardSpeed);

	m_pVFX_Trail = dynamic_pointer_cast<CVFX_TrailLine>(m_pVFX_Trail_Pool->Pop(make_pair(shared_from_gameobject(), (const _char*)(nullptr))));
	m_pVFX_Trail->Set_Color(_color(0.99f, 0.95f, 0.70f, 1.00f));
	m_pVFX_Trail->Set_Thickness(0.03f);
	m_pVFX_Trail->Set_Interval(0.0005f);

	return S_OK;
}

HRESULT CRailGunner_PistolBullet::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pTransform = Get_Component<CTransform>(COMPONENT::TRANSFORM);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Ready_Components", "Nullptr Exception: m_pShader");
	}

	return S_OK;
}

HRESULT CRailGunner_PistolBullet::Ready_Behaviors()
{
	if (FAILED(__super::Ready_Behaviors()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Ready_Behaviors", "Failed to __super::Ready_Behaviors");
	}

	m_pPhysics = Get_Behavior<CPhysics>(BEHAVIOR::PHYSICS);
	if (nullptr == m_pPhysics)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Ready_Behaviors", "Nullptr Exception: m_pPhysics");
	}

	return S_OK;
}

void CRailGunner_PistolBullet::Set_Target(shared_ptr<CMonster> _pTargetMonster)
{
	m_eState			= STATE::TARGET;
	m_pTarget			= _pTargetMonster;
	m_pTargetTransform	= _pTargetMonster->Get_Component<CTransform>(COMPONENT::TRANSFORM);
}

void CRailGunner_PistolBullet::Manage_State(_float _fTimeDelta)
{
	m_fTimeAcc += _fTimeDelta;
	if (m_fTimeAcc >= PISTOLBULLET_LIFESPAN)
	{
		Destroy();
		return;
	}

	m_pTransform->LookTo(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::LOOK), true, true);

	switch (m_eState)
	{
	case STATE::UNTARGET:
	{
	}
	break;
	case STATE::TARGET:
	{
		m_pPhysics->Flattern(true, true, true);
		m_pPhysics->Force(m_pTargetTransform->Get_State(TRANSFORM::POSITION) - m_pTransform->Get_State(TRANSFORM::POSITION), m_tEntityDesc.fForwardSpeed * 50.f, _fTimeDelta);
		if (Function::Distance(m_pTransform, m_pTargetTransform) < 10.f)
		{
			m_pTarget->Hit();
			m_pPool->Push(shared_from_gameobject());
		}
	}
	break;
	}
}

void CRailGunner_PistolBullet::Search_Target()
{
	CGameInstance::Get_Instance()->Iterate_Pools(CGameInstance::Get_Instance()->Current_Scene(),
		[&](pair<wstring, shared_ptr<CObjectPool>> pair)->_bool
		{
			pair.second->Iterate_Objects(
				[&](shared_ptr<CGameObject> pObject)->_bool
				{
					shared_ptr<CMonster> pMonster = dynamic_pointer_cast<CMonster>(pObject);
					if (nullptr != pMonster)
					{
						_float fDistance = Function::Distance(m_pTransform, pMonster->Get_Component<CTransform>(COMPONENT::TRANSFORM));
						if (PISTOLBULLET_TARGET_RANGE > fDistance)
						{
							if (m_pTargetTransform)
							{
								if (Function::Distance(m_pTransform, m_pTargetTransform) > fDistance)
								{
									Set_Target(pMonster);
								}
							}
							else
							{
								Set_Target(pMonster);
							}
						}
					}

					return true;
				}
			);

			return true;
		}
	);
}

void CRailGunner_PistolBullet::Destroy()
{
	m_pPool->Push(shared_from_gameobject());
	m_pPool				= nullptr;
	m_fTimeAcc			= 0.f;
	m_eState			= STATE::UNTARGET;
	m_pTargetTransform	= nullptr;
	m_pTarget			= nullptr;

	m_pVFX_Trail->Push_Pool(m_pVFX_Trail_Pool);
	m_pVFX_Trail = nullptr;
}

shared_ptr<CRailGunner_PistolBullet> CRailGunner_PistolBullet::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRailGunner_PistolBullet> pInstance = make_private_shared(CRailGunner_PistolBullet, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CRailGunner_PistolBullet::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRailGunner_PistolBullet::Clone(any)
{
	shared_ptr<CRailGunner_PistolBullet> pInstance = make_private_shared_copy(CRailGunner_PistolBullet, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRailGunner_PistolBullet::Create", "Failed to Initialize");
	}

	return pInstance;
}
