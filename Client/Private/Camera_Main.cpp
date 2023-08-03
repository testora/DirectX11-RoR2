#include "ClientPCH.h"
#include "Camera_Main.h"
#include "GameInstance.h"

CCamera_Main::CCamera_Main(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCamera(_pDevice, _pContext)
{
}

HRESULT CCamera_Main::Initialize(any _arg)
{
	CCamera_Main::CAMERA_MAIN_DESC tCameraMainDesc{};
	tCameraMainDesc.tCameraDesc.eType = CCamera::TYPE::PERSPECTIVE;

	tCameraMainDesc.tCameraDesc.vEye = _float3(0.f, 10.f, 0.f);
	tCameraMainDesc.tCameraDesc.vAt = _float3(0.f, 0.f, 1.f);

	tCameraMainDesc.tCameraDesc.fFovAngleY = XMConvertToRadians(75.f);
	tCameraMainDesc.tCameraDesc.fAspect = static_cast<_float>(g_iWinCX) / static_cast<_float>(g_iWinCY);

	tCameraMainDesc.tCameraDesc.fNear = 0.1f;
	tCameraMainDesc.tCameraDesc.fFar = 1000.f;

	m_tCameraMainDesc = tCameraMainDesc;

	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM);
	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());

	if (FAILED(__super::Initialize(m_tCameraMainDesc.tCameraDesc)))
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Initialize", "Failed to __super::Initialize");
	}

	return S_OK;
}

void CCamera_Main::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CCamera_Main::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	m_pRendererCom->Add_RenderGroup(RENDER_GROUP::PRIORITY, shared_from_this());
}

HRESULT CCamera_Main::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Render", "Failed to __super::Render");
	}

	return S_OK;
}

shared_ptr<CCamera_Main> CCamera_Main::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, any _arg)
{
	shared_ptr<CCamera_Main> pInstance = make_private_shared(CCamera_Main, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_arg)))
	{
		MSG_RETURN(nullptr, "CCamera_Main::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCamera_Main::Clone(any)
{
	return shared_from_this();
}
