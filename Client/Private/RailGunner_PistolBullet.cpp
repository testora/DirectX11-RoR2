#include "ClientPCH.h"
#include "RailGunner_PistolBullet.h"
#include "GameInstance.h"
#include "Camera_Main.h"

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
	tColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	tColliderDesc.fRadius	= 0.5f;

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);
	m_umapComponentArg[COMPONENT::COLLIDER]			= make_pair(PROTOTYPE_COMPONENT_COLLIDER, tColliderDesc);

	m_pTexMask = CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Bullet/texRailgunPistolSquare.png"));

	return S_OK;
}

HRESULT CRailGunner_PistolBullet::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_PistolBullet::Initialize", "__super::Initialize Failed");
	}

	return S_OK;
}

void CRailGunner_PistolBullet::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	m_pTransform->LookTo(CPipeLine::Get_Instance()->Get_Transform(TRANSFORM::LOOK), true, true);
}

void CRailGunner_PistolBullet::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::PRIORITY);
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

HRESULT CRailGunner_PistolBullet::Fetch(any _aPosition)
{
	m_pTransform->Set_State(TRANSFORM::POSITION, any_cast<_float3>(_aPosition));

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
