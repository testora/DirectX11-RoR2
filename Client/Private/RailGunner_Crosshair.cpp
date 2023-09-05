#include "ClientPCH.h"
#include "RailGunner_Crosshair.h"
#include "GameInstance.h"

#define MAIN_INBOUNDSCALE		_float3(384.f, 128.f, 1.f) + _float3(7.f, 7.f, 0.f)
#define MAIN_OUTBOUNDSCALE		MAIN_INBOUNDSCALE + _float3(8.f, 8.f, 0.f)
#define MAIN_BRACKETSCALE		_float3(134.f, 70.f, 1.f)
#define MAIN_NIBSCALE			_float3(103.f, 70.f, 1.f)
#define MAIN_BRACKETSTEP		_float3(150.f, 35.f, 1.f)
#define MAIN_NIBSTEP			_float3(120.f, 35.f, 1.f)
#define MAIN_FLAVORSCALE		_float3(16.f, 32.f, 1.f)
#define MAIN_FLAVORPOSITION		_float4(220.f, 185.f, 0.f, 1.f)
#define MAIN_FLAVORROTATESPEED	1000.f

#define SCOPE_CROSSHAIRSCALE	_float3(128.f, 128.f, 1.f)
#define SCOPE_INBOUNDSCALE		_float3(750.f, 424.f, 1.f) + _float3(7.f, 7.f, 0.f)
#define SCOPE_OUTBOUNDSCALE		SCOPE_INBOUNDSCALE + _float3(8.f, 8.f, 0.f)

#define SPRINT_ARROWSCALE		_float3(98.f, 98.f, 1.f)

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

	for (size_t i = 0; i < IDX(ELEMENT::MAX); ++i)
	{
		m_pTransform[i] = CTransform::Create(m_pDevice, m_pContext);
	}

	m_pTexBounds		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBounds.png"));
	m_pTexBracket		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBracket%d.png"), 2);
	m_pTexFlavor		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerFlavor1.png"));

	m_pTexScope			= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texRailgunnerScope%d.png"), 2);
	m_pTexBounds_Glowy	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBoundsGlowy.png"));

	m_pTexDot			= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairDot.png"));
	m_pTexArrow			= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairArrow.png"));

	m_pTransform[IDX(ELEMENT::SCOPE)]->Set_Scale(_float3(static_cast<_float>(g_iWinCX), static_cast<_float>(g_iWinCY), 1.f));
	m_pTransform[IDX(ELEMENT::SCOPE)]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	m_pTransform[IDX(ELEMENT::CROSS)]->Set_Scale(SCOPE_CROSSHAIRSCALE);
	m_pTransform[IDX(ELEMENT::CROSS)]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	m_pTransform[IDX(ELEMENT::ARROW)]->Set_Scale(SPRINT_ARROWSCALE);
	m_pTransform[IDX(ELEMENT::NIBS)]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	return S_OK;
}

void CRailGunner_Crosshair::Tick(_float _fTimeDelta)
{
	if (m_bitElement.test(IDX(ELEMENT::FLAVOR)))
	{
		m_pTransform[IDX(ELEMENT::FLAVOR)]->Rotate(TRANSFORM::LOOK, _fTimeDelta * m_fFlavorRotateSpeed);
	}
	
	if (CGameInstance::Get_Instance()->Key_Down('9'))
	{
		Change_State(STATE::MAIN);
	}
	
	if (CGameInstance::Get_Instance()->Key_Down('8'))
	{
		Change_State(STATE::SPRINT);
	}

	if (CGameInstance::Get_Instance()->Key_Down('7'))
	{
		Change_State(STATE::SNIPER);
	}
}

void CRailGunner_Crosshair::Late_Tick(_float _fTimeDelta)
{
	m_pRenderer->Add_RenderObject(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CRailGunner_Crosshair::Render()
{
#pragma region Bounds
	if (m_bitElement.test(IDX(ELEMENT::BOUND_IN))
	||	m_bitElement.test(IDX(ELEMENT::BOUND_OUT)))
	{
		if (FAILED(m_pTexBounds->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (m_bitElement.test(IDX(ELEMENT::BOUND_IN)))
		{
			if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::BOUND_IN)], 2)))
			{
				MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
			}
		}
		if (m_bitElement.test(IDX(ELEMENT::BOUND_OUT)))
		{
			if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::BOUND_OUT)], 2)))
			{
				MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
			}
		}
	}
#pragma endregion
#pragma region Bracket
	if (m_bitElement.test(IDX(ELEMENT::BRACKET)))
	{
		if (FAILED(m_pTexBracket->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::BRACKET)], 3)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
		}
		if (m_bitElement.test(IDX(ELEMENT::NIBS)))
		{
			if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::NIBS)], 4)))
			{
				MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
			}
		}
	}
#pragma endregion
#pragma region Flavor
	if (m_bitElement.test(IDX(ELEMENT::FLAVOR)))
	{
		if (FAILED(m_pTexFlavor->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::FLAVOR)], 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGuner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
		}
	}
#pragma endregion
#pragma region Bound Glowy
	if (m_bitElement.test(IDX(ELEMENT::BOUND_GLOWY_IN))
	||	m_bitElement.test(IDX(ELEMENT::BOUND_GLOWY_OUT)))
	{
		if (FAILED(m_pTexBounds_Glowy->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (m_bitElement.test(IDX(ELEMENT::BOUND_GLOWY_IN)))
		{
			if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::BOUND_GLOWY_IN)], 2)))
			{
				MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
			}
		}
		if (m_bitElement.test(IDX(ELEMENT::BOUND_GLOWY_OUT)))
		{
			if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::BOUND_GLOWY_OUT)], 2)))
			{
				MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
			}
		}
	}
#pragma endregion
#pragma region Scope
	if (m_bitElement.test(IDX(ELEMENT::SCOPE)))
	{
		if (FAILED(m_pTexScope->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::CROSS)], 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGuner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
		}
		if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::SCOPE)], 5)))
		{
			MSG_RETURN(E_FAIL, "CRailGuner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
		}
	}
#pragma endregion
#pragma region Arrow
	if (m_bitElement.test(IDX(ELEMENT::ARROW)))
	{
		if (FAILED(m_pTexArrow->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(ELEMENT::ARROW)], 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGuner_Crosshair::Render", "Failed to CVIBuffer_Rect::Bind_OnShader");
		}
	}
#pragma endregion

	return S_OK;
}

void CRailGunner_Crosshair::Change_State(const STATE _eState)
{
	m_eState = _eState;

	m_bitElement.reset();

	switch (_eState)
	{
	case STATE::MAIN:
		Init_Bound();
		Init_Bracket();
		Init_Flavor();
		break;

	case STATE::SNIPER:
		Init_Scope();
		Init_Bound_Glowy();
		break;

	case STATE::SUPER_CHARGE:
		break;

	case STATE::SUPER_READY:
		break;

	case STATE::SUPER_REBOOT:
		break;

	case STATE::SPRINT:
		Init_Arrow();
		break;
	}
}

void CRailGunner_Crosshair::Init_Bound()
{
	m_bitElement.set(IDX(ELEMENT::BOUND_IN));
	m_bitElement.set(IDX(ELEMENT::BOUND_OUT));

	_float	fInAcc(0.f), fOutAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fInAcc < 1.f)
			{
				fInAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::BOUND_IN)]->Set_Scale(Function::Lerp(_float3(), MAIN_INBOUNDSCALE, Function::Clamp(0.f, 1.f, fInAcc)));
			}
			if (fOutAcc < 1.f)
			{
				fOutAcc += _fTimeDelta / 0.15f;
				m_pTransform[IDX(ELEMENT::BOUND_OUT)]->Set_Scale(Function::Lerp(_float3(), MAIN_OUTBOUNDSCALE, Function::Clamp(0.f, 1.f, fOutAcc)));
			}

			if (fInAcc >= 1.f && fOutAcc >= 1.f)
			{
				return false;
			}

			return true;
		}
	);
}

void CRailGunner_Crosshair::Init_Flavor()
{
	m_bitElement.set(IDX(ELEMENT::FLAVOR));

	m_fFlavorRotateSpeed = MAIN_FLAVORROTATESPEED;

	_float fAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 1.f)
			{
				fAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::FLAVOR)]->Set_Scale(Function::Lerp(_float3(), MAIN_FLAVORSCALE, Function::Clamp(0.f, 1.f, fAcc)));
				m_pTransform[IDX(ELEMENT::FLAVOR)]->Set_State(TRANSFORM::POSITION, Function::Lerp(_float4(0.f, 0.f, 0.f, 1.f), MAIN_FLAVORPOSITION, Function::Clamp(0.f, 1.f, fAcc)));
			}

			if (fAcc >= 1.f)
			{
				return false;
			}

			return true;
		}
	);
}

void CRailGunner_Crosshair::Init_Bracket()
{
	m_bitElement.set(IDX(ELEMENT::BRACKET));
	m_bitElement.set(IDX(ELEMENT::NIBS));

	_float	fBracketAcc(0.f), fNibsAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fBracketAcc < 0.5f)
			{
				fBracketAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::BRACKET)]->Set_Scale(Function::Lerp(_float3(), MAIN_BRACKETSTEP, Function::Clamp(0.f, 1.f, fBracketAcc * 2.f)));
			}
			else if (fBracketAcc < 1.f)
			{
				fBracketAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::BRACKET)]->Set_Scale(Function::Lerp(MAIN_BRACKETSTEP, MAIN_BRACKETSCALE, Function::Clamp(0.f, 1.f, fBracketAcc * 2.f - 1.f)));
			}

			if (fNibsAcc < 0.5f)
			{
				fNibsAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::NIBS)]->Set_Scale(Function::Lerp(_float3(), MAIN_NIBSTEP, Function::Clamp(0.f, 1.f, fNibsAcc * 2.f)));
			}
			else if (fNibsAcc < 1.f)
			{
				fNibsAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::NIBS)]->Set_Scale(Function::Lerp(MAIN_NIBSTEP, MAIN_NIBSCALE, Function::Clamp(0.f, 1.f, fNibsAcc * 2.f - 1.f)));
			}

			if (fBracketAcc >= 1.f && fNibsAcc >= 1.f)
			{
				return false;
			}

			return true;
		}
	);
}

void CRailGunner_Crosshair::Init_Scope()
{
	m_bitElement.set(IDX(ELEMENT::SCOPE));
	m_bitElement.set(IDX(ELEMENT::CROSS));
}

void CRailGunner_Crosshair::Init_Bound_Glowy()
{
	m_bitElement.set(IDX(ELEMENT::BOUND_GLOWY_IN));
	m_bitElement.set(IDX(ELEMENT::BOUND_GLOWY_OUT));

	_float	fInAcc(0.f), fOutAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fInAcc < 1.f)
			{
				fInAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::BOUND_GLOWY_IN)]->Set_Scale(Function::Lerp(_float3(), SCOPE_INBOUNDSCALE, Function::Clamp(0.f, 1.f, fInAcc)));
			}
			if (fOutAcc < 1.f)
			{
				fOutAcc += _fTimeDelta / 0.15f;
				m_pTransform[IDX(ELEMENT::BOUND_GLOWY_OUT)]->Set_Scale(Function::Lerp(_float3(), SCOPE_OUTBOUNDSCALE, Function::Clamp(0.f, 1.f, fOutAcc)));
			}

			if (fInAcc >= 1.f && fOutAcc >= 1.f)
			{
				return false;
			}

			return true;
		}
	);
}

void CRailGunner_Crosshair::Init_Arrow()
{
	m_bitElement.set(IDX(ELEMENT::ARROW));
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
