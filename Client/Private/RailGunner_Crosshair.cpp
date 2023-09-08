#include "ClientPCH.h"
#include "RailGunner_Crosshair.h"
#include "GameInstance.h"
#include "Camera_Main.h"

#define MAIN_INBOUND_SCALE			_float3(_float3(384.f, 128.f, 1.f) + _float3(7.f, 7.f, 0.f))
#define MAIN_OUTBOUND_SCALE			_float3(MAIN_INBOUND_SCALE + _float3(8.f, 8.f, 0.f))
#define MAIN_BRACKET_SCALE			_float3(134.f, 70.f, 1.f)
#define MAIN_BRACKET_STEP			_float3(150.f, 35.f, 1.f)
#define MAIN_BRACKET_BOUNCE_SCALE	_float3(1.4f, 1.f, 1.f)
#define MAIN_NIB_SCALE				_float3(103.f, 70.f, 1.f)
#define MAIN_NIB_STEP				_float3(120.f, 35.f, 1.f)
#define MAIN_FLAVOR_SCALE			_float3(16.f, 32.f, 1.f)
#define MAIN_FLAVOR_POSITION		_float4(210.f, 90.f, 0.f, 1.f)
#define MAIN_FLAVOR_ROTATESPEED		1000.f

#define SCOPE_CROSSHAIR_SCALE		_float3(128.f, 128.f, 1.f)
#define SCOPE_INBOUND_SCALE			_float3(_float3(750.f, 424.f, 1.f) + _float3(7.f, 7.f, 0.f))
#define SCOPE_OUTBOUND_SCALE		_float3(SCOPE_INBOUND_SCALE + _float3(12.f, 12.f, 0.f))

#define	RELOAD_POSITION				_float4(0.f, -140.f, 0.f, 1.f)
#define	RELOAD_BOUND_SCALE			_float3(_float3(393.f, 37.f, 1.f) + _float3(7.f, 7.f, 0.f))
#define	RELOAD_STRIPE_SCALE			_float3(393.f, 44.f, 1.f)
#define RELOAD_BAR_SCALE			_float3(384.f, 8.f, 1.f)
#define RELOAD_SLIDE_SCALE			_float3(5.f, 23.f, 1.f)
#define RELOAD_BOOST_SCALE			_float3(35.f, 35.f, 1.f)
#define RELOAD_BOOST_POSITION		_float4(RELOAD_POSITION + _float4(-214.f, 0.f, 0.f, 1.f))
#define RELOAD_TAG_SUCCESS_SCALE	_float3(1.33f, 0.33f, 1.f)

#define SPRINT_ARROW_SCALE			_float3(98.f, 98.f, 1.f)

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

	m_pairTagPositionRange							= make_pair(0.42f, 0.17f);

#pragma region Texture

	for (size_t i = 0; i < IDX(ELEMENT::MAX); ++i)
	{
		m_pTransform[i] = CTransform::Create(m_pDevice, m_pContext);
	}

	m_pTexSinglePixel	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/White1px.png"));

	m_pTexBounds		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBounds.png"));
	m_pTexBracket		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBracket%d.png"), 2);
	m_pTexFlavor		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerFlavor1.png"));

	m_pTexScope			= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texRailgunnerScope%d.png"), 2);
	m_pTexBounds_Glowy	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunnerBoundsGlowy.png"));

	m_pTexSniperCharge	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texSniperCharge.png"));

	m_pTexStripes		= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunneStripes.png"));
	m_pTexBoost			= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairRailgunBoosted.png"));

	m_pTexDot			= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairDot.png"));
	m_pTexArrow			= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/RailGunner/Crosshair/texCrosshairArrow.png"));

#pragma endregion
#pragma region Transform

	m_pTransform[IDX(ELEMENT::MAIN_BRACKET)]->Set_Scale(MAIN_BRACKET_SCALE);
	m_pTransform[IDX(ELEMENT::MAIN_NIBS)]->Set_Scale(MAIN_NIB_SCALE);

	m_pTransform[IDX(ELEMENT::MAIN_NIBS)]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	m_pTransform[IDX(ELEMENT::SCOPE_SCOPE)]->Set_Scale(_float3(static_cast<_float>(g_iWinCX), static_cast<_float>(g_iWinCY), 1.f));
	m_pTransform[IDX(ELEMENT::SCOPE_SCOPE)]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));
	m_pTransform[IDX(ELEMENT::SCOPE_CROSS)]->Set_Scale(SCOPE_CROSSHAIR_SCALE);
	m_pTransform[IDX(ELEMENT::SCOPE_CROSS)]->Set_State(TRANSFORM::POSITION, _float4(0.f, 0.f, 0.f, 1.f));

	m_pTransform[IDX(ELEMENT::RELOAD_BOUND)]->Set_Scale(RELOAD_BOUND_SCALE);
	m_pTransform[IDX(ELEMENT::RELOAD_BOUND)]->Set_State(TRANSFORM::POSITION, RELOAD_POSITION);
	m_pTransform[IDX(ELEMENT::RELOAD_STRIPE)]->Set_Scale(RELOAD_STRIPE_SCALE);
	m_pTransform[IDX(ELEMENT::RELOAD_STRIPE)]->Set_State(TRANSFORM::POSITION, RELOAD_POSITION);
	m_pTransform[IDX(ELEMENT::RELOAD_BAR)]->Set_Scale(RELOAD_BAR_SCALE);
	m_pTransform[IDX(ELEMENT::RELOAD_BAR)]->Set_State(TRANSFORM::POSITION, RELOAD_POSITION);
	m_pTransform[IDX(ELEMENT::RELOAD_TAG)]->Set_Scale(_float3(m_pairTagPositionRange.second * RELOAD_BAR_SCALE.x, RELOAD_BAR_SCALE.y, RELOAD_BAR_SCALE.z));
	m_pTransform[IDX(ELEMENT::RELOAD_TAG)]->Set_State(TRANSFORM::POSITION, _float4((m_pairTagPositionRange.first - 0.5f) * RELOAD_BAR_SCALE.x, RELOAD_POSITION.y, 0.f, 1.f));
	m_pTransform[IDX(ELEMENT::RELOAD_BOOST)]->Set_Scale(RELOAD_BOOST_SCALE);
	m_pTransform[IDX(ELEMENT::RELOAD_BOOST)]->Set_State(TRANSFORM::POSITION, RELOAD_BOOST_POSITION);
	m_pTransform[IDX(ELEMENT::RELOAD_SLIDE)]->Set_Scale(RELOAD_SLIDE_SCALE);

	m_pTransform[IDX(ELEMENT::ARROW)]->Set_Scale(SPRINT_ARROW_SCALE);

#pragma endregion
#pragma region Diffuse

	m_vDiffuse[IDX(ELEMENT::MAIN_BOUND_IN)]		= _color(0.84f, 0.73f, 0.30f, 1.00f);
	m_vDiffuse[IDX(ELEMENT::MAIN_BOUND_OUT)]	= _color(0.84f, 0.73f, 0.30f, 0.50f);
	m_vDiffuse[IDX(ELEMENT::MAIN_BRACKET)]		= _color(1.00f, 1.00f, 1.00f, 1.00f);
	m_vDiffuse[IDX(ELEMENT::MAIN_NIBS)]			= _color(1.00f, 1.00f, 1.00f, 1.00f);
	m_vDiffuse[IDX(ELEMENT::MAIN_FLAVOR)]		= _color(1.00f, 1.00f, 1.00f, 1.00f);

	m_vDiffuse[IDX(ELEMENT::SCOPE_SCOPE)]		= _color(1.00f, 1.00f, 1.00f, 1.00f);
	m_vDiffuse[IDX(ELEMENT::SCOPE_CROSS)]		= _color(1.00f, 1.00f, 1.00f, 1.00f);
	m_vDiffuse[IDX(ELEMENT::SCOPE_BOUND_IN)]	= _color(0.95f, 0.67f, 0.76f, 1.00f);
	m_vDiffuse[IDX(ELEMENT::SCOPE_BOUND_OUT)]	= _color(0.95f, 0.67f, 0.76f, 0.50f);

	m_vDiffuse[IDX(ELEMENT::RELOAD_STRIPE)]		= _color(0.85f, 0.55f, 0.70f, 0.25f);
	m_vDiffuse[IDX(ELEMENT::RELOAD_BAR)]		= _color(0.00f, 0.00f, 0.00f, 0.50f);
	m_vDiffuse[IDX(ELEMENT::RELOAD_SLIDE)]		= _color(1.00f, 0.86f, 0.92f, 1.00f);

	m_vDiffuse[IDX(ELEMENT::DOT)]				= _color(1.00f, 1.00f, 1.00f, 1.00f);

	m_vDiffuse[IDX(ELEMENT::ARROW)]				= _color(1.00f, 1.00f, 1.00f, 1.00f);

#pragma endregion
	return S_OK;
}

HRESULT CRailGunner_Crosshair::Initialize(any)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Initialize", "Failed to __super::Initialize");
	}

	Change_State(CROSSHAIR::MAIN);

	return S_OK;

}
void CRailGunner_Crosshair::Tick(_float _fTimeDelta)
{
	if (m_bitElement.test(IDX(ELEMENT::MAIN_FLAVOR)))
	{
		m_pTransform[IDX(ELEMENT::MAIN_FLAVOR)]->Rotate(TRANSFORM::LOOK, _fTimeDelta * MAIN_FLAVOR_ROTATESPEED);
	}

	if (m_bitElement.test(IDX(ELEMENT::RELOAD_SLIDE)))
	{
		m_pTransform[IDX(ELEMENT::RELOAD_SLIDE)]->Set_State(TRANSFORM::POSITION,
			_float4((m_fCurrentTagPosition - 0.5f) * RELOAD_BAR_SCALE.x, RELOAD_POSITION.y, 0.f, 1.f));
	}

	if (CGameInstance::Get_Instance()->Key_Down('9'))
	{
		Change_State(CROSSHAIR::MAIN);
	}
	
	if (CGameInstance::Get_Instance()->Key_Down('8'))
	{
		Change_State(CROSSHAIR::SPRINT);
	}

	if (CGameInstance::Get_Instance()->Key_Down('7'))
	{
		Change_State(CROSSHAIR::SCOPE);
	}

	if (CGameInstance::Get_Instance()->Key_Down('6'))
	{
		Bounce_Bracket();
	}

	if (CGameInstance::Get_Instance()->Key_Down('5'))
	{
		Change_State(CROSSHAIR::RELOAD);
	}

	if (CGameInstance::Get_Instance()->Key_Down('4'))
	{
		m_bHitTag = true;
	}
}

void CRailGunner_Crosshair::Late_Tick(_float _fTimeDelta)
{
	m_pRenderer->Add_RenderObject(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CRailGunner_Crosshair::Render()
{
#pragma region Bounds
	if (m_bitElement.test(IDX(ELEMENT::MAIN_BOUND_IN))
	||	m_bitElement.test(IDX(ELEMENT::MAIN_BOUND_OUT)))
	{
		if (FAILED(m_pTexBounds->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}

		if (FAILED(Render_Element(ELEMENT::MAIN_BOUND_IN, 2)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: MAIN_BOUND_IN");
		}
		if (FAILED(Render_Element(ELEMENT::MAIN_BOUND_OUT, 2)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: MAIN_BOUND_OUT");
		}
	}
#pragma endregion
#pragma region Bracket
	if (m_bitElement.test(IDX(ELEMENT::MAIN_BRACKET)))
	{
		if (FAILED(m_pTexBracket->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::MAIN_BRACKET, 3)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: MAIN_BRACKET");
		}
		if (FAILED(Render_Element(ELEMENT::MAIN_NIBS, 4)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: MAIN_NIBS");
		}
	}
#pragma endregion
#pragma region Flavor
	if (m_bitElement.test(IDX(ELEMENT::MAIN_FLAVOR)))
	{
		if (FAILED(m_pTexFlavor->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::MAIN_FLAVOR, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: MAIN_FLAVOR");
		}
	}
#pragma endregion
#pragma region Scope
	if (m_bitElement.test(IDX(ELEMENT::SCOPE_SCOPE)))
	{
		if (FAILED(m_pTexScope->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::SCOPE_SCOPE, 5)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: SCOPE_SCOPE");
		}
		if (FAILED(Render_Element(ELEMENT::SCOPE_CROSS, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: SCOPE_CROSS");
		}
	}
#pragma endregion
#pragma region Bound Glowy
	if (m_bitElement.test(IDX(ELEMENT::SCOPE_BOUND_IN))
	||	m_bitElement.test(IDX(ELEMENT::SCOPE_BOUND_OUT)))
	{
		if (FAILED(m_pTexBounds_Glowy->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::SCOPE_BOUND_IN, 2)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: SCOPE_BOUND_IN");
		}
		if (FAILED(Render_Element(ELEMENT::SCOPE_BOUND_OUT, 2)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: SCOPE_BOUND_OUT");
		}
	}
#pragma endregion
#pragma region Reload
	if (m_bitElement.test(IDX(ELEMENT::RELOAD_BOUND)))
	{
		if (FAILED(m_pTexBounds_Glowy->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::RELOAD_BOUND, 2)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: RELOAD_BOUND");
		}
	}

	if (m_bitElement.test(IDX(ELEMENT::RELOAD_STRIPE)))
	{
		if (FAILED(m_pTexStripes->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::RELOAD_STRIPE, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: RELOAD_STRIPE");
		}
	}

	if (m_bitElement.test(IDX(ELEMENT::RELOAD_BAR))
	||	m_bitElement.test(IDX(ELEMENT::RELOAD_TAG))
	||	m_bitElement.test(IDX(ELEMENT::RELOAD_SLIDE)))
	{
		if (FAILED(m_pTexSinglePixel->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::RELOAD_BAR, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: RELOAD_BAR");
		}
		if (FAILED(Render_Element(ELEMENT::RELOAD_TAG, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: RELOAD_TAG");
		}
		if (FAILED(Render_Element(ELEMENT::RELOAD_SLIDE, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: RELOAD_SLIDE");
		}
	}

	if (m_bitElement.test(IDX(ELEMENT::RELOAD_BOOST)))
	{
		if (FAILED(m_pTexBoost->Bind_ShaderResourceViews(m_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to CTexture::Bind_ShaderResourceViews");
		}
		if (FAILED(Render_Element(ELEMENT::RELOAD_BOOST, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: RELOAD_BOOST");
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
		if (FAILED(Render_Element(ELEMENT::ARROW, 1)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render", "Failed to Render_Element: ARROW");
		}
	}
#pragma endregion

	return S_OK;
}

void CRailGunner_Crosshair::Change_State(const CROSSHAIR _eState)
{
	if (m_eState == _eState)
	{
		return;
	}

	m_bitElement.reset();

	if (CROSSHAIR::SCOPE == m_eState && CROSSHAIR::SCOPE != _eState)
	{
		CPipeLine::Get_Instance()->Get_Camera<CCamera_Main>()->Release_FOV();
	}

	switch (_eState)
	{
	case CROSSHAIR::MAIN:
		Visualize_Main();
		if (CROSSHAIR::RELOAD != m_eState)
		{
			Visualize_Bracket();
		}
		Visualize_Flavor();
		break;

	case CROSSHAIR::SCOPE:
		CPipeLine::Get_Instance()->Get_Camera<CCamera_Main>()->Adjust_FOV(RAILGUNNER_SCOPE_FOV, RAILGUNNER_SCOPE_ZOOM_IN_DURATION, RAILGUNNER_SCOPE_ZOOM_WEIGHT);
		Visualize_Scope();
		break;

	case CROSSHAIR::SUPER_CHARGE:

		break;

	case CROSSHAIR::SUPER_READY:

		break;

	case CROSSHAIR::SUPER_REBOOT:

		break;

	case CROSSHAIR::RELOAD:
		Visualize_Reload();
		Visualize_Bracket();
		break;

	case CROSSHAIR::SPRINT:
		Visualize_Sprint();
		break;
	}

	m_eState = _eState;
}

void CRailGunner_Crosshair::Bounce_Bracket()
{
	m_pTransform[IDX(ELEMENT::MAIN_BRACKET)]->Set_Scale(Function::Lerp(MAIN_BRACKET_SCALE * MAIN_BRACKET_BOUNCE_SCALE, MAIN_BRACKET_SCALE, 0.5f, 0.5f));
	
	if (m_bitElement.test(IDX(ELEMENT::MAIN_BRACKET)))
	{
		_float	fAcc(0.f);
		CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
			[=](_float _fTimeDelta) mutable->_bool
			{
				if (fAcc < 1.f)
				{
					fAcc += _fTimeDelta / 0.1f;
					m_pTransform[IDX(ELEMENT::MAIN_BRACKET)]->Set_Scale(Function::Lerp(MAIN_BRACKET_SCALE * MAIN_BRACKET_BOUNCE_SCALE, MAIN_BRACKET_SCALE, Function::Clamp(0.f, 1.f, fAcc), 0.5f));
				}

				return !(fAcc >= 1.f);
			}
		);
	}
}

void CRailGunner_Crosshair::Hit_Reload()
{
	m_bHitTag = true;
}

void CRailGunner_Crosshair::Visualize_Main()
{
	m_bitElement.set(IDX(ELEMENT::MAIN_BOUND_IN));
	m_bitElement.set(IDX(ELEMENT::MAIN_BOUND_OUT));
	m_bitElement.set(IDX(ELEMENT::MAIN_BRACKET));
	m_bitElement.set(IDX(ELEMENT::MAIN_NIBS));

	_float	fInAcc(0.f), fOutAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fInAcc < 1.f)
			{
				fInAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::MAIN_BOUND_IN)]->Set_Scale(Function::Lerp(_float3(), MAIN_INBOUND_SCALE, Function::Clamp(0.f, 1.f, fInAcc)));
			}
			if (fOutAcc < 1.f)
			{
				fOutAcc += _fTimeDelta / 0.15f;
				m_pTransform[IDX(ELEMENT::MAIN_BOUND_OUT)]->Set_Scale(Function::Lerp(_float3(), MAIN_OUTBOUND_SCALE, Function::Clamp(0.f, 1.f, fOutAcc)));
			}

			return !(fInAcc >= 1.f && fOutAcc >= 1.f);
		}
	);

	m_vDiffuse[IDX(ELEMENT::MAIN_FLAVOR)] = m_vDiffuse[IDX(ELEMENT::MAIN_BOUND_IN)];
}

void CRailGunner_Crosshair::Visualize_Bracket()
{
	m_bitElement.set(IDX(ELEMENT::MAIN_BRACKET));
	m_bitElement.set(IDX(ELEMENT::MAIN_NIBS));

	_float	fBracketAcc(0.f), fNibsAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fBracketAcc > 0.5f)
			{
				fBracketAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::MAIN_BRACKET)]->Set_Scale(Function::Lerp(MAIN_BRACKET_STEP, MAIN_BRACKET_SCALE, Function::Clamp(0.f, 1.f, fBracketAcc * 2.f - 1.f)));
			}
			else if (fBracketAcc > 0.f)
			{
				fBracketAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::MAIN_BRACKET)]->Set_Scale(Function::Lerp(_float3(), MAIN_BRACKET_STEP, Function::Clamp(0.f, 1.f, fBracketAcc * 2.f)));
			}

			if (fNibsAcc > 0.5f)
			{
				fNibsAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::MAIN_NIBS)]->Set_Scale(Function::Lerp(MAIN_NIB_STEP, MAIN_NIB_SCALE, Function::Clamp(0.f, 1.f, fNibsAcc * 2.f - 1.f)));
			}
			else if (fNibsAcc > 0.f)
			{
				fNibsAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::MAIN_NIBS)]->Set_Scale(Function::Lerp(_float3(), MAIN_NIB_STEP, Function::Clamp(0.f, 1.f, fNibsAcc * 2.f)));
			}

			return !(fBracketAcc >= 1.f && fNibsAcc >= 1.f);
		}
	);
}

void CRailGunner_Crosshair::Visualize_Flavor()
{
	m_bitElement.set(IDX(ELEMENT::MAIN_FLAVOR));

	_float fAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 1.f)
			{
				fAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::MAIN_FLAVOR)]->Set_Scale(Function::Lerp(_float3(), MAIN_FLAVOR_SCALE, Function::Clamp(0.f, 1.f, fAcc)));
				m_pTransform[IDX(ELEMENT::MAIN_FLAVOR)]->Set_State(TRANSFORM::POSITION, Function::Lerp(_float4(0.f, 0.f, 0.f, 1.f), MAIN_FLAVOR_POSITION, Function::Clamp(0.f, 1.f, fAcc)));
			}

			return !(fAcc >= 1.f);
		}
	);
}

void CRailGunner_Crosshair::Visualize_Scope()
{
	m_bitElement.set(IDX(ELEMENT::SCOPE_SCOPE));
	m_bitElement.set(IDX(ELEMENT::SCOPE_CROSS));
	m_bitElement.set(IDX(ELEMENT::SCOPE_BOUND_IN));
	m_bitElement.set(IDX(ELEMENT::SCOPE_BOUND_OUT));

	_float	fInAcc(0.f), fOutAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fInAcc < 1.f)
			{
				fInAcc += _fTimeDelta / 0.1f;
				m_pTransform[IDX(ELEMENT::SCOPE_BOUND_IN)]->Set_Scale(Function::Lerp(_float3(), SCOPE_INBOUND_SCALE, Function::Clamp(0.f, 1.f, fInAcc)));
			}
			if (fOutAcc < 1.f)
			{
				fOutAcc += _fTimeDelta / 0.15f;
				m_pTransform[IDX(ELEMENT::SCOPE_BOUND_OUT)]->Set_Scale(Function::Lerp(_float3(), SCOPE_OUTBOUND_SCALE, Function::Clamp(0.f, 1.f, fOutAcc)));
			}

			return !(fInAcc >= 1.f && fOutAcc >= 1.f);
		}
	);
}

void CRailGunner_Crosshair::Visualize_Reload()
{
	m_bitElement.set(IDX(ELEMENT::RELOAD_BOUND));
	m_bitElement.set(IDX(ELEMENT::RELOAD_STRIPE));
	m_bitElement.set(IDX(ELEMENT::RELOAD_BAR));
	m_bitElement.set(IDX(ELEMENT::RELOAD_TAG));
	m_bitElement.set(IDX(ELEMENT::RELOAD_SLIDE));
	m_bitElement.set(IDX(ELEMENT::RELOAD_BOOST));

	m_vDiffuse[IDX(ELEMENT::RELOAD_BOUND)]	= _color(0.85f, 0.55f, 0.70f, 0.75f);
	m_vDiffuse[IDX(ELEMENT::RELOAD_TAG)]	= _color(1.00f, 0.58f, 0.69f, 1.00f);
	m_vDiffuse[IDX(ELEMENT::RELOAD_BOOST)]	= _color(1.00f, 0.53f, 0.55f, 1.00f);

	m_pTransform[IDX(ELEMENT::RELOAD_BOUND)]->Set_Scale(RELOAD_BOUND_SCALE);
	m_pTransform[IDX(ELEMENT::RELOAD_STRIPE)]->Set_Scale(RELOAD_STRIPE_SCALE);
	m_pTransform[IDX(ELEMENT::RELOAD_SLIDE)]->Set_Scale(RELOAD_SLIDE_SCALE);

	m_bHitTag = false;

	_float fAcc(0.f), fHitAcc(0.f), fMissAcc(0.f);
	CGameInstance::Get_Instance()->Register_TickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (!m_bHitTag)
			{
				if (fAcc < 1.f)
				{
					fAcc += _fTimeDelta / RAILGUNNER_RELOAD_COOL;
				}

				m_fCurrentTagPosition = Function::Clamp(0.f, 1.f, fAcc);

				if (Function::InRange(m_fCurrentTagPosition, m_pairTagPositionRange.first - m_pairTagPositionRange.second * 0.5f, m_pairTagPositionRange.first + m_pairTagPositionRange.second * 0.5f))
				{
					m_pTransform[IDX(ELEMENT::RELOAD_SLIDE)]->Set_Scale(
						Function::Lerp(_float3(RELOAD_SLIDE_SCALE.x, RELOAD_SLIDE_SCALE.y * 2.f, 1.f), RELOAD_SLIDE_SCALE,
							Function::Clamp(0.f, 1.f, (m_fCurrentTagPosition - m_pairTagPositionRange.first + m_pairTagPositionRange.second * 0.5f) / (m_pairTagPositionRange.second)), 0.5f));
				}
			}
			else
			{
				if (Function::InRange(m_fCurrentTagPosition, m_pairTagPositionRange.first - m_pairTagPositionRange.second * 0.5f, m_pairTagPositionRange.first + m_pairTagPositionRange.second * 0.5f))
				{
					if (!fMissAcc)
					{
						if (fHitAcc < 1.f)
						{
							fHitAcc += _fTimeDelta / RAILGUNNER_RELOAD_SUCCESS_DELAY;
						}

						m_vDiffuse[IDX(ELEMENT::RELOAD_BOUND)]	= _color(1.00f, 1.00f, 1.00f, 1.00f);
						m_vDiffuse[IDX(ELEMENT::RELOAD_TAG)]	= _color(1.00f, 1.00f, 1.00f, 1.00f);
						m_vDiffuse[IDX(ELEMENT::RELOAD_BOOST)]	= _color(1.00f, 1.00f, 1.00f, 1.00f);

						m_pTransform[IDX(ELEMENT::RELOAD_BOUND)]->Set_Scale(
							Function::Lerp(RELOAD_BOUND_SCALE * RELOAD_TAG_SUCCESS_SCALE, RELOAD_BOUND_SCALE, Function::Clamp(0.f, 1.f, fHitAcc)));
						m_pTransform[IDX(ELEMENT::RELOAD_SLIDE)]->Set_Scale(
							Function::Lerp(RELOAD_SLIDE_SCALE * _float3(1.f, RELOAD_TAG_SUCCESS_SCALE.y, 1.f), RELOAD_SLIDE_SCALE, Function::Clamp(0.f, 1.f, fHitAcc)));
						m_pTransform[IDX(ELEMENT::RELOAD_BOOST)]->Set_State(TRANSFORM::POSITION,
							Function::Lerp(RELOAD_BOOST_POSITION * _float3(RELOAD_TAG_SUCCESS_SCALE.x, 1.f, 1.f), RELOAD_BOOST_POSITION, Function::Clamp(0.f, 1.f, fHitAcc)));
					}
					else
					{
						if (fAcc < 1.f)
						{
							fAcc += _fTimeDelta / RAILGUNNER_RELOAD_COOL;
						}

						if (fMissAcc < 1.f)
						{
							fMissAcc += _fTimeDelta / RAILGUNNER_RELOAD_FAIL;
						}

						m_fCurrentTagPosition = Function::Clamp(0.f, 1.f, fAcc);

						m_pTransform[IDX(ELEMENT::RELOAD_BOUND)]->Set_Scale(
							Function::Lerp(_float3(RELOAD_BOUND_SCALE.x, 0.f, 1.f), _float3(RELOAD_BOUND_SCALE.x, RELOAD_BOUND_SCALE.y * 0.5f, 1.f), Function::Clamp(0.f, 1.f, fMissAcc)));
					}
				}
				else
				{
					if (fAcc < 1.f)
					{
						fAcc += _fTimeDelta / RAILGUNNER_RELOAD_COOL;
					}

					if (fMissAcc < 1.f)
					{
						fMissAcc += _fTimeDelta / RAILGUNNER_RELOAD_FAIL;
					}

					m_vDiffuse[IDX(ELEMENT::RELOAD_BOUND)]	= _color(1.00f, 0.10f, 0.10f, 1.00f);
					m_vDiffuse[IDX(ELEMENT::RELOAD_TAG)]	= _color(1.00f, 0.10f, 0.10f, 1.00f);
					m_vDiffuse[IDX(ELEMENT::RELOAD_BOOST)]	= _color(1.00f, 0.10f, 0.10f, 1.00f);

					m_fCurrentTagPosition = Function::Clamp(0.f, 1.f, fAcc);

					m_pTransform[IDX(ELEMENT::RELOAD_BOUND)]->Set_Scale(
						Function::Lerp(_float3(RELOAD_BOUND_SCALE.x, 0.f, 1.f), _float3(RELOAD_BOUND_SCALE.x, RELOAD_BOUND_SCALE.y * 0.5f, 1.f), Function::Clamp(0.f, 1.f, fMissAcc)));
					m_pTransform[IDX(ELEMENT::RELOAD_SLIDE)]->Set_Scale(RELOAD_SLIDE_SCALE * _float3(1.f, 0.5f, 1.f));
					m_pTransform[IDX(ELEMENT::RELOAD_STRIPE)]->Set_Scale(RELOAD_STRIPE_SCALE * _float3(1.f, 0.5f, 1.f));

				}
			}

			if (fAcc >= 1.f || fHitAcc >= 1.f)
			{
				Change_State(CROSSHAIR::MAIN);

				return false;
			}

			return true;
		}
	);
}

void CRailGunner_Crosshair::Visualize_Sprint()
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

HRESULT CRailGunner_Crosshair::Render_Element(const ELEMENT _eElement, _uint _iPassIndex)
{
	if (m_bitElement.test(IDX(_eElement)))
	{
		if (FAILED(m_pShader->Bind_Vector(SHADER_MTRLDIF, m_vDiffuse[IDX(_eElement)])))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render_Element", "Failed to CShader::Bind_Vector: SHADER_MTRLDIF");
		}

		if (FAILED(m_pVIBuffer->Render(m_pShader, m_pTransform[IDX(_eElement)], _iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CRailGunner_Crosshair::Render_Element", "Failed to CVIBuffer_Rect::Bind_OnShader");
		}
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