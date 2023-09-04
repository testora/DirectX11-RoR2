#include "ClientPCH.h"
#include "RailGunner_Crosshair.h"
#include "GameInstance.h"

CRailGunner_Crosshair::CRailGunner_Crosshair(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CRailGunner_Crosshair::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	return S_OK;
}

HRESULT CRailGunner_Crosshair::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Initialize", "Failed to __super::Initialize");
	}

	m_pTransform[BOUND_IN]	= CTransform::Create(m_pDevice, m_pContext);
	m_pTransform[BOUND_OUT]	= CTransform::Create(m_pDevice, m_pContext);

	m_pTexBounds	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBounds.png"));
	m_pTexBracket	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBracket%d.png"), 2);
	
	const _float2 vInBoundsSize		= _float2(768.f, 256.f);
	const _float2 vOutBoundsSize	= _float2(16.f, 16.f) + vInBoundsSize;

	m_pTransform[BOUND_IN]->Set_Matrix(g_mUnit);
	m_pTransform[BOUND_IN]->Set_Scale(vInBoundsSize);
	m_pTransform[BOUND_IN]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	m_pTransform[BOUND_OUT]->Set_Matrix(g_mUnit);
	m_pTransform[BOUND_OUT]->Set_Scale(vOutBoundsSize);
	m_pTransform[BOUND_IN]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CRailGunner_Crosshair::Tick(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Down('6'))
	{
		Init_Bound();
	}
}

void CRailGunner_Crosshair::Late_Tick(_float _fTimeDelta)
{
	m_pRenderer->Add_RenderObject(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CRailGunner_Crosshair::Render()
{
#pragma region Bounds
	if (FAILED(m_pTexBounds->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
	}
	if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[BOUND_IN], 2)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
	}
	if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[BOUND_OUT], 2)))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
	}
#pragma endregion
#pragma region Bracket
#pragma endregion

	return S_OK;
}

void CRailGunner_Crosshair::Init_Bound()
{
	_float	fInAcc(0.f), fOutAcc(0.f);
	const _float3 vInBoundsSize		= _float3(768.f, 256.f, 1.f);
	const _float3 vOutBoundsSize	= _float3(16.f, 16.f, 0.f) + vInBoundsSize;

	m_pTransform[BOUND_IN]->Set_Matrix(g_mUnit);
	m_pTransform[BOUND_IN]->Set_Scale(_float3(0.1f, 0.1f, 1.f));
	m_pTransform[BOUND_IN]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	m_pTransform[BOUND_OUT]->Set_Matrix(g_mUnit);
	m_pTransform[BOUND_OUT]->Set_Scale(_float3(0.1f, 0.1f, 1.f));
	m_pTransform[BOUND_OUT]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fInAcc < 1.f)
			{
				fInAcc += _fTimeDelta / 0.1f;
				m_pTransform[BOUND_IN]->Set_Scale(Function::Lerp(_float3(), vInBoundsSize, Function::Clamp(0.f, 1.f, fInAcc)));
			}
			if (fOutAcc < 1.f)
			{
				fOutAcc += _fTimeDelta / 0.2f;
				m_pTransform[BOUND_OUT]->Set_Scale(Function::Lerp(_float3(), vOutBoundsSize, Function::Clamp(0.f, 1.f, fOutAcc)));
			}

			if (fInAcc >= 1.f && fOutAcc >= 1.f)
			{
				return false;
			}

			return true;
		}
	);
}

void CRailGunner_Crosshair::Init_Bracket()
{
}

HRESULT CRailGunner_Crosshair::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pRenderer = Get_Component<CRenderer>(COMPONENT::RENDERER);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Ready_Components", "Nullptr Exception: m_pRenderer");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Ready_Components", "Nullptr Exception: m_pShader");
	}

	m_pVIBuffer = Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER_RECT);
	if (nullptr == m_pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Ready_Components", "Nullptr Exception: m_pVIBuffer");
	}

	return S_OK;
}

shared_ptr<CRailGunner_Crosshair> CRailGunner_Crosshair::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CRailGunner_Crosshair> pInstance = make_private_shared(CRailGunner_Crosshair, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CRailGunner_Crosshair::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRailGunner_Crosshair::Clone(any)
{
	shared_ptr<CRailGunner_Crosshair> pInstance = make_private_shared_copy(CRailGunner_Crosshair, *this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CRailGunner_Crosshair::Clone", "Failed to Initialize");
	}

	return pInstance;
}
