#include "ClientPCH.h"
#include "BrotherUI_HealthBar.h"
#include "GameInstance.h"

#define BACKGROUND_SCALE	_float3(960.f, 24.f, 1.f)
#define BACKGROUND_POSITION	_float4(0.f, 320.f, 0.f, 1.f)

#define HEALTH_SCALE		_float3(860.f, 16.f, 1.f)

CBrotherUI_HealthBar::CBrotherUI_HealthBar(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

HRESULT CBrotherUI_HealthBar::Initialize_Prototype()
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::SHADER)	| BIT(COMPONENT::VIBUFFER_RECT);

	m_umapComponentArg[COMPONENT::RENDERER]			= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]			= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);
	m_umapComponentArg[COMPONENT::VIBUFFER_RECT]	= make_pair(PROTOTYPE_COMPONENT_VIBUFFER_RECT, g_aNull);

	return S_OK;
}

HRESULT CBrotherUI_HealthBar::Initialize(any _pBrotherEntityDesc)
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CBrotherUI_HealthBar::Initialize", "Failed to __super::Initialize");
	}

	if (_pBrotherEntityDesc.has_value())
	{
		m_pEntityDesc = any_cast<ENTITYDESC*>(_pBrotherEntityDesc);
	}

	m_pBackground	= CTransform::Create(m_pDevice, m_pContext);
	m_pHealth		= CTransform::Create(m_pDevice, m_pContext);

	m_pTexHealth	= CTexture::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Texture/White1px.png"));

	m_pBackground->Set_Scale(BACKGROUND_SCALE);
	m_pBackground->Set_State(TRANSFORM::POSITION, BACKGROUND_POSITION);

	m_pVIBuffer			= Get_Component<CVIBuffer_Rect>(COMPONENT::VIBUFFER);
	if (nullptr == m_pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CBrotherUI_HealthBar::Initialize", "Failed to Get_Component: CVIBuffer_Rect");
	}

	return S_OK;
}

void CBrotherUI_HealthBar::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	_float	fHPFactor(m_pEntityDesc->fHP / m_pEntityDesc->fMaxHP);
	_float3	vHealthScale(HEALTH_SCALE), vHealthPosition(BACKGROUND_POSITION);

	vHealthScale.x		*=	fHPFactor;
	vHealthPosition.x = HEALTH_SCALE.x * Function::Lerp(-0.5f, 0.f, std::clamp(fHPFactor, 0.f, 1.f));

	m_pHealth->Set_Scale(vHealthScale);
	m_pHealth->Set_State(TRANSFORM::POSITION, vHealthPosition);
}

void CBrotherUI_HealthBar::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::UI);
}

HRESULT CBrotherUI_HealthBar::Render()
{
	if (FAILED(m_pVIBuffer->Render(m_pShader, m_pBackground, 7)))
	{
		MSG_RETURN(E_FAIL, "CBrotherUI_HealthBar::Render", "Failed to CVIBuffer_Rect::Render");
	}

	if (FAILED(m_pVIBuffer->Render(m_pShader, m_pHealth, 8)))
	{
		MSG_RETURN(E_FAIL, "CBrotherUI_HealthBar::Render", "Failed to CVIBuffer_Rect::Render");
	}

	return S_OK;
}

HRESULT CBrotherUI_HealthBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CBrotherUI_HealthBar::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pShader = Get_Component<CShader>(COMPONENT::SHADER);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CBrotherUI_HealthBar::Ready_Components", "Failed to Get_Component: SHADER");
	}

	return S_OK;
}

shared_ptr<CBrotherUI_HealthBar> CBrotherUI_HealthBar::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CBrotherUI_HealthBar> pInstance = make_private_shared(CBrotherUI_HealthBar, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_RETURN(nullptr, "CBrotherUI_HealthBar::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CBrotherUI_HealthBar::Clone(any _pBrotherEntityDesc)
{
	shared_ptr<CBrotherUI_HealthBar> pInstance = make_private_shared_copy(CBrotherUI_HealthBar, *this);

	if (FAILED(pInstance->Initialize(_pBrotherEntityDesc)))
	{
		MSG_RETURN(nullptr, "CBrotherUI_HealthBar::Clone", "Failed to Initialize");
	}

	return pInstance;
}
