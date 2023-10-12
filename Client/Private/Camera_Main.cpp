#include "ClientPCH.h"
#include "Camera_Main.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"

CCamera_Main::CCamera_Main(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CCamera(_pDevice, _pContext)
{
}

HRESULT CCamera_Main::Initialize(any _aDesc)
{
	m_umapComponentArg[COMPONENT::RENDERER]	= make_pair(PROTOTYPE_COMPONENT_RENDERER_MAIN, g_aNull);
	m_umapComponentArg[COMPONENT::SHADER]	= make_pair(PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX, g_aNull);

	if (_aDesc.has_value())
	{
		m_tCameraMainDesc = any_cast<CAMERA_MAIN_DESC>(_aDesc);
	}
	else
	{
		m_tCameraMainDesc.tCameraDesc.eType			= CCamera::TYPE::PERSPECTIVE;

		m_tCameraMainDesc.tCameraDesc.vEye			= _float4(0.f, 10.f, 0.f, 1.f);
		m_tCameraMainDesc.tCameraDesc.vAt			= _float4(0.f, 0.f, 0.f, 0.f);

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
	m_pPipeLine->Set_Camera(static_pointer_cast<CCamera>(shared_from_this()));

	m_vSensitivty = _float2(MAINCAM_SENSITIVITY_YAW, MAINCAM_SENSITIVITY_PITCH);

	return S_OK;
}

void CCamera_Main::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

#ifdef _DEBUG
	if (false == m_bDebug)
	{
#endif
		Handle_MouseInput(_fTimeDelta);

		if (nullptr != m_pTargetTransform)
		{
			Smooth_Tranformation(_fTimeDelta);
		}
#ifdef _DEBUG
	}
	else
	{
		Handle_MouseInput_Debug(_fTimeDelta);
	}
#endif
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

		ImGui::Checkbox("Debug", &m_bDebug);

		ImGui::End();
	}
#endif
}

HRESULT CCamera_Main::Render()
{
	if (FAILED(__super::Render()))
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Render", "Failed to __super::Render");
	}

	return S_OK;
}

HRESULT CCamera_Main::Attach(shared_ptr<CTransform> _pTargetTransform, _float4 _vOffset)
{
	if (nullptr == _pTargetTransform)
	{
		MSG_RETURN(E_FAIL, "CCamera_Main::Attach", "Null Exception: CTransform");
	}

	m_pTargetTransform	= _pTargetTransform;
	m_vMainOffset		= _vOffset;

	return S_OK;
}

void CCamera_Main::Rebound_Pistol()
{
#ifdef _DEBUG
	if (m_bDebug)
	{
		return;
	}
#endif

	_float4 vPistolReboundQuaternion = QuaternionBetweenAxis(
		XMVector3TransformNormal(_float3(0.f, 1.f, 0.f), m_pTransform->Get_Matrix()),
		XMVector3TransformNormal(_float3(0.02f, 1.00f, -0.05f), m_pTransform->Get_Matrix()));

	_float fAcc(0.f);
	CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 0.5f)
			{
				fAcc += _fTimeDelta / MAINCAM_PISTOL_REBOUND_DURATION;
		
				_float fRatio(_fTimeDelta / MAINCAM_PISTOL_REBOUND_DURATION);
				if (fAcc > 0.5f)
				{
					fRatio -= (fAcc - 0.5f) * 2.f;
				}
		
				m_pTransform->Rotate(Function::Slerp(vPistolReboundQuaternion, fRatio));
			}
			else if (fAcc < 1.f)
			{
				fAcc += _fTimeDelta / MAINCAM_PISTOL_REBOUND_DURATION;
		
				_float fRatio(_fTimeDelta / MAINCAM_PISTOL_REBOUND_DURATION);
				if (fAcc > 1.f)
				{
					fRatio -= fAcc - 1.f;
				}
		
				m_pTransform->Rotate(Function::Slerp(vPistolReboundQuaternion, -fRatio));
			}

			return fAcc < 1.f;
		}
	);
}

void CCamera_Main::Rebound_Sniper()
{
#ifdef _DEBUG
	if (m_bDebug)
	{
		return;
	}
#endif
	_float4 vSniperReboundQuaternion = QuaternionBetweenAxis(
		XMVector3TransformNormal(_float3(0.f, 1.f, 0.f), m_pTransform->Get_Matrix()),
		XMVector3TransformNormal(_float3(0.00f, 1.00f, -0.02f), m_pTransform->Get_Matrix()));

	_float fAcc(0.f);
	CGameInstance::Get_Instance()->Register_OnTickListener(shared_from_this(),
		[=](_float _fTimeDelta) mutable->_bool
		{
			if (fAcc < 1.f)
			{
				fAcc += _fTimeDelta / 0.4f;

				m_pTransform->Rotate(Function::Slerp(vSniperReboundQuaternion, (-powf(fAcc, 0.25f) + 0.85f)) * 1.5f);
			}

		//	if (fAcc < 0.5f)
		//	{
		//		fAcc += _fTimeDelta / 0.55f;
		//
		//		_float fRatio(_fTimeDelta / 0.55f);
		//		if (fAcc > 0.5f)
		//		{
		//			fRatio -= (fAcc - 0.5f) * 2.f;
		//		}
		//
		//		m_pTransform->Rotate(Function::Slerp(m_vSniperReboundQuaternion, fRatio, 0.5f));
		//	}
		//	else if (fAcc < 1.f)
		//	{
		//		fAcc += _fTimeDelta / 0.55f;
		//
		//		_float fRatio(_fTimeDelta / 0.55f);
		//		if (fAcc > 1.f)
		//		{
		//			fRatio -= fAcc - 1.f;
		//		}
		//
		//		m_pTransform->Rotate(Function::Slerp(m_vSniperReboundQuaternion, -fRatio, 0.5f));
		//	}

			return fAcc < 1.f;
		}
	);
}

void CCamera_Main::Adjust_FOV(_float _fRadian, _float _fDuration, _float _fWeight, _float2 _vSensitivity)
{
#ifdef _DEBUG
	if (m_bDebug)
	{
		return;
	}
#endif

	__super::Adjust_FOV(_fRadian, _fDuration, _fWeight);

	m_vSensitivty =	_vSensitivity;
}

void CCamera_Main::Release_FOV(_float _fDuration, _float _fWeight)
{
#ifdef _DEBUG
	if (m_bDebug)
	{
		return;
	}
#endif

	__super::Release_FOV(_fDuration, _fWeight);

	m_vSensitivty = _float2(MAINCAM_SENSITIVITY_YAW, MAINCAM_SENSITIVITY_PITCH);
}

void CCamera_Main::Handle_MouseInput(_float _fTimeDelta)
{
	POINT ptCursorMove{};

	if (!CGameInstance::Get_Instance()->Is_CursorOn())
	{
		ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();
	}

	if (ptCursorMove.x)
	{
		m_pTransform->Rotate(_float3(0.f, 1.f, 0.f), m_vSensitivty.x * ptCursorMove.x * _fTimeDelta);
	}

	if (ptCursorMove.y)
	{
		_float3	vLook		= m_pTransform->Get_State(TRANSFORM::LOOK);
		_float	fCurPitch	= atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
		_float	fChgPitch	= m_vSensitivty.y * ptCursorMove.y * _fTimeDelta;
		_float	fNewPitch	= Function::Clamp(XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX), fCurPitch + fChgPitch);
		_float	fFinal		= fNewPitch - fCurPitch;

		m_pTransform->Rotate(TRANSFORM::RIGHT, fFinal);
	}
}

#ifdef _DEBUG
void CCamera_Main::Handle_MouseInput_Debug(_float _fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Hold(VK_RBUTTON))
	{
		POINT ptCursorMove = CGameInstance::Get_Instance()->Get_CursorMove();

		if (ptCursorMove.x)
		{
			m_pTransform->Rotate(_float3(0.f, 1.f, 0.f), m_vSensitivty.x * ptCursorMove.x * _fTimeDelta);
		}

		if (ptCursorMove.y)
		{
			_float3	vLook		= m_pTransform->Get_State(TRANSFORM::LOOK);
			_float	fCurPitch	= atan2f(-vLook.y, sqrtf(powf(vLook.x, 2) + powf(vLook.z, 2)));
			_float	fChgPitch	= m_vSensitivty.y * ptCursorMove.y * _fTimeDelta;
			_float	fNewPitch	= Function::Clamp(XMConvertToRadians(MAINCAM_PITCH_MIN), XMConvertToRadians(MAINCAM_PITCH_MAX), fCurPitch + fChgPitch);
			_float	fFinal		= fNewPitch - fCurPitch;

			m_pTransform->Rotate(TRANSFORM::RIGHT, fFinal);
		}

		if (_short sVerticalScroll = CGameInstance::Get_Instance()->Get_CursorScroll().y)
		{
			m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::LOOK) * sVerticalScroll * 0.01f);
		}
	}
	if (CGameInstance::Get_Instance()->Key_Hold(VK_MBUTTON))
	{
		POINT ptMove = CGameInstance::Get_Instance()->Get_CursorMove();
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::RIGHT)	* m_vDebugSensitivity.x * static_cast<_float>(-ptMove.x));
		m_pTransform->Translate(m_pTransform->Get_State(TRANSFORM::UP)		* m_vDebugSensitivity.y * static_cast<_float>(-ptMove.y));
	}
}
#endif

void CCamera_Main::Smooth_Tranformation(_float _fTimeDelta)
{
	_float3 vLook	= m_pTransform->Get_State(TRANSFORM::LOOK).normalize();
	_float3 vOffset	= XMVector3Rotate(_float3(m_vMainOffset.x, 0.f, m_vMainOffset.z), QuaternionBetweenAxis(_float3(0.f, 0.f, 1.f), vLook));

	m_pTransform->Set_State(TRANSFORM::POSITION, Function::Lerp(m_pTransform->Get_State(TRANSFORM::POSITION),
		_float3(m_pTargetTransform->Get_State(TRANSFORM::POSITION) + _float3(0.f, m_vMainOffset.y, 0.f)) + vOffset, 0.5f));
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
	return shared_from_gameobject();
}
