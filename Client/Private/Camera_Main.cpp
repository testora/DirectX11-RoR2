#include "ClientPCH.h"
#include "Camera_Main.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"

CCamera_Main::CCamera_Main(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCamera(_pDevice, _pContext)
{
}

HRESULT CCamera_Main::Initialize(any _desc)
{
	if (_desc.has_value())
	{
		m_tCameraMainDesc = any_cast<CAMERA_MAIN_DESC>(_desc);
	}
	else
	{
		m_tCameraMainDesc.tCameraDesc.eType			= CCamera::TYPE::PERSPECTIVE;

		m_tCameraMainDesc.tCameraDesc.vEye			= _float4(0.f, 10.f, 0.f, 1.f);
		m_tCameraMainDesc.tCameraDesc.vAt			= _float4(0.f, 0.f, 0.f, 1.f);

		m_tCameraMainDesc.tCameraDesc.fFovAngleY	= XMConvertToRadians(60.f);
		m_tCameraMainDesc.tCameraDesc.fAspect		= static_cast<_float>(g_iWinCX) / static_cast<_float>(g_iWinCY);

		m_tCameraMainDesc.tCameraDesc.fNear			= 0.1f;
		m_tCameraMainDesc.tCameraDesc.fFar			= 1000.f;
	}

	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM);
	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, any());

	if (FAILED(__super::Initialize(m_tCameraMainDesc.tCameraDesc)))
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Initialize", "Failed to __super::Initialize");
	}

	CGameInstance::Get_Instance()->Fix_Cursor();

	return S_OK;
}

void CCamera_Main::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	Debug_MouseControl(_fTimeDelta);
	Debug_KeyControl(_fTimeDelta);
}

void CCamera_Main::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

#ifdef _DEBUG
	if (ACTIVATE_IMGUI)
	{
		if (CImGui_Manager::Get_Instance()->Is_Enable())
		{
			ImGui::Begin("MainCam");
			ImGui::Text("Position: ");
			ImGui::Text("X: %f\t", m_pTransformCom->Get_State(CTransform::POSITION).x);
			ImGui::SameLine();
			ImGui::Text("Y: %f\t", m_pTransformCom->Get_State(CTransform::POSITION).y);
			ImGui::SameLine();
			ImGui::Text("Z: %f\t", m_pTransformCom->Get_State(CTransform::POSITION).z);
			ImGui::End();
		}
	}
#endif

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

void CCamera_Main::Debug_MouseControl(_float _fTimeDelta)
{
	POINT ptCursorMove{};

	if (!CGameInstance::Get_Instance()->Is_CursorOn() || CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_MOUSE_CONTROL))
	{
		ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();
	}

	if (ptCursorMove.x)
	{
		m_pTransformCom->Rotate(_float3(0.f, 1.f, 0.f), MAINCAM_SENSITIVITY_YAW * ptCursorMove.x * _fTimeDelta);
	}

	if (ptCursorMove.y)
	{
		_float3	vLook		= m_pTransformCom->Get_State(CTransform::STATE::LOOK);
		_float	fCurPitch	= atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
		_float	fChgPitch	= MAINCAM_SENSITIVITY_PITCH * ptCursorMove.y * _fTimeDelta;
		_float	fNewPitch	= Function::Clamp(fCurPitch + fChgPitch, XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX));
		_float	fFinal		= fNewPitch - fCurPitch;

		m_pTransformCom->Rotate(m_pTransformCom->Get_State(CTransform::STATE::RIGHT), fFinal);
	}
}

void CCamera_Main::Debug_KeyControl(_float _fTimeDelta)
{
	_float3 vMove{};

	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_FORWARD))
	{
		vMove += m_pTransformCom->Get_State(CTransform::STATE::LOOK);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_BACKWARD))
	{
		vMove -= m_pTransformCom->Get_State(CTransform::STATE::LOOK);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_LEFT))
	{
		vMove -= m_pTransformCom->Get_State(CTransform::STATE::RIGHT);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_RIGHT))
	{
		vMove += m_pTransformCom->Get_State(CTransform::STATE::RIGHT);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_UP))
	{
		vMove += _float3(0.f, 1.f, 0.f);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_DOWN))
	{
		vMove -= _float3(0.f, 1.f, 0.f);
	}

	if (vMove != _float3(0.f, 0.f, 0.f))
	{
		m_pTransformCom->Translate(MAINCAM_DEBUG_SPEED * vMove * _fTimeDelta);
	}
}

shared_ptr<CCamera_Main> CCamera_Main::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	shared_ptr<CCamera_Main> pInstance = make_private_shared(CCamera_Main, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_RETURN(nullptr, "CCamera_Main::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CGameObject> CCamera_Main::Clone(any)
{
	return shared_from_this();
}
