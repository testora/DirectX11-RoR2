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
	m_bitComponent |= BIT(COMPONENT::RENDERER) | BIT(COMPONENT::TRANSFORM);

	m_umapComponentArg[COMPONENT::RENDERER] = make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);

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
		m_tCameraMainDesc.tCameraDesc.fFar			= 10000.f;
	}

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

	if (nullptr != m_pTargetTransform)
	{
	//	m_pTransform->Set_Matrix(m_vOffset * m_pTargetTransform->Get_Matrix());
		Smooth_Tranformation(_fTimeDelta);
	}

	Debug_MouseControl(_fTimeDelta);
	Debug_KeyControl(_fTimeDelta);
}

void CCamera_Main::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

#if ACTIVATE_IMGUI
	if (CImGui_Manager::Get_Instance()->Is_Enable())
	{
		ImGui::Begin("MainCam");
		ImGui::Text("Position: ");
		ImGui::Text("X: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).x);
		ImGui::SameLine();
		ImGui::Text("Y: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).y);
		ImGui::SameLine();
		ImGui::Text("Z: %f\t", m_pTransform->Get_State(TRANSFORM::POSITION).z);
		ImGui::End();
	}
#endif

	m_pRenderer->Add_RenderObject(RENDER_GROUP::CAMERA, shared_from_this());
}

HRESULT CCamera_Main::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CCamera_Main::Attach(shared_ptr<class CTransform> _pTargetTransform, _float4x4 _mOffset)
{
	if (nullptr == _pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Attach", "Null Exception: CTransform");
	}

	m_pTargetTransform	= _pTargetTransform;
	m_mOffset			= _mOffset;

	return S_OK;
}

void CCamera_Main::Adjust_FOV(_float _fRadian, _float _fDuration, _float _fWeight)
{
	__super::Adjust_FOV(_fRadian, _fDuration, _fWeight);
}

void CCamera_Main::Release_FOV(_float _fDuration, _float _fWeight)
{
	__super::Release_FOV(_fDuration, _fWeight);
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
		m_pTransform->Rotate(_float3(0.f, 1.f, 0.f), MAINCAM_SENSITIVITY_YAW * ptCursorMove.x * _fTimeDelta);
	}

	if (ptCursorMove.y)
	{
		_float3	vLook		= m_pTransform->Get_State(TRANSFORM::LOOK);
		_float	fCurPitch	= atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
		_float	fChgPitch	= MAINCAM_SENSITIVITY_PITCH * ptCursorMove.y * _fTimeDelta;
		_float	fNewPitch	= Function::Clamp(XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX), fCurPitch + fChgPitch);
		_float	fFinal		= fNewPitch - fCurPitch;

		m_pTransform->Rotate(m_pTransform->Get_State(TRANSFORM::RIGHT), fFinal);
	}
}

void CCamera_Main::Debug_KeyControl(_float _fTimeDelta)
{
	_float3 vMove{};

	if (CGameInstance::Get_Instance()->Is_CursorOn())
	{
		return;
	}

	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_FORWARD))
	{
		vMove += m_pTransform->Get_State(TRANSFORM::LOOK);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_BACKWARD))
	{
		vMove -= m_pTransform->Get_State(TRANSFORM::LOOK);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_LEFT))
	{
		vMove -= m_pTransform->Get_State(TRANSFORM::RIGHT);
	}
	if (CGameInstance::Get_Instance()->Key_Hold(MAINCAM_DEBUG_RIGHT))
	{
		vMove += m_pTransform->Get_State(TRANSFORM::RIGHT);
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
		m_pTransform->Translate(MAINCAM_DEBUG_SPEED * vMove * _fTimeDelta);
	}
}

void CCamera_Main::Smooth_Tranformation(_float _fTimeDelta)
{
	m_pTransform->Set_Matrix(Function::Lerp(m_pTransform->Get_Matrix(), m_mOffset * m_pTargetTransform->Get_Matrix(), 1.f, false, true, true));
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
