#include "EnginePCH.h"
#include "Camera.h"
#include "PipeLine.h"
#include "Event_Handler.h"
#include "Component_Manager.h"

CCamera::CCamera(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject	(_pDevice, _pContext)
	, m_pPipeLine	(CPipeLine::Get_Instance())
	, m_mView		(g_mUnit)
	, m_mProjection	(g_mUnit)
{
}

HRESULT CCamera::Initialize(any _arg)
{
	m_bitComponent	|= BIT(COMPONENT::RENDERER)	| BIT(COMPONENT::TRANSFORM) |	BIT(COMPONENT::SHADER);

	m_tCameraDesc	= any_cast<CAMERA_DESC>(_arg);
	
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Failed to Initialize");
	}

	switch (m_tCameraDesc.eType)
	{
	case TYPE::PERSPECTIVE:
	{
		m_mProjection	= XMMatrixPerspectiveFovLH(m_tCameraDesc.fFovAngleY, m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar);

		m_fTargetFovAngleY = m_tCameraDesc.fFovAngleY;
		m_pTransform->Set_State(TRANSFORM::POSITION, m_tCameraDesc.vEye);
		m_pTransform->LookAt(m_tCameraDesc.vAt, false);
	}
	break;
	case TYPE::ORTHOGONAL:
	{
		m_mView			= XMMatrixLookAtLH(m_tCameraDesc.vEye, m_tCameraDesc.vAt, m_tCameraDesc.vUp);
		m_mProjection	= XMMatrixOrthographicLH(m_tCameraDesc.fWidth, m_tCameraDesc.fHeight, m_tCameraDesc.fNear, m_tCameraDesc.fFar);
	}
	break;

	default:
		MSG_RETURN(E_FAIL, "CCamera::Initialize", "Invalid Camera Type");
	}

	Render();

#if WIP_FRUSTRUM_CULLING
	m_pPipeLine->Update_Frustum();
#endif

	return S_OK;
}

void CCamera::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CCamera::Late_Tick(_float _fTimeDelta)
{
	__super::Late_Tick(_fTimeDelta);

	Add_RenderObject(RENDER_GROUP::CAMERA);
}

HRESULT CCamera::Render()
{
	switch (m_tCameraDesc.eType)
	{
	case TYPE::PERSPECTIVE:
		m_pPipeLine->Set_Transform(PIPELINE::WORLD,			m_pTransform->Get_Matrix());
		m_pPipeLine->Set_Transform(PIPELINE::VIEW,			m_pTransform->Get_Matrix().inverse());
		m_pPipeLine->Set_Transform(PIPELINE::PROJECTION,	m_mProjection);
		break;

	case TYPE::ORTHOGONAL:
		m_pPipeLine->Set_Transform(PIPELINE::WORLD,			m_pTransform->Get_Matrix());
		m_pPipeLine->Set_Transform(PIPELINE::VIEW,			m_mView);
		m_pPipeLine->Set_Transform(PIPELINE::PROJECTION,	m_mProjection);
		break;
	}

	return S_OK;
}

HRESULT CCamera::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
	{
		MSG_RETURN(E_FAIL, "CCamera::Ready_Components", "Failed to __super::Ready_Components");
	}

	m_pRenderer = dynamic_pointer_cast<CRenderer>(m_umapComponent[COMPONENT::RENDERER]);
	if (nullptr == m_pRenderer)
	{
		MSG_RETURN(E_FAIL, "CCamera::Ready_Components", "Nullptr Exception: m_pRenderer");
	}

	m_pTransform = dynamic_pointer_cast<CTransform>(m_umapComponent[COMPONENT::TRANSFORM]);
	if (nullptr == m_pTransform)
	{
		MSG_RETURN(E_FAIL, "CCamera::Ready_Components", "Nullptr Exception: m_pTransform");
	}

	m_pShader = dynamic_pointer_cast<CShader>(m_umapComponent[COMPONENT::SHADER]);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CCamera::Ready_Components", "Nullptr Exception: m_pShader");
	}

	return S_OK;
}

void CCamera::Adjust_FOV(_float _fRadian, _float _fDuration, _float _fWeight)
{
	if (TYPE::ORTHOGONAL == m_tCameraDesc.eType)
	{
		MSG_RETURN(, "CCamera::Adjust_FOV", "Orthogonal Camera does not support Adjust_FOV");
	}

	m_fTargetFovAngleY = _fRadian;

	if (!_fDuration)
	{
		m_mProjection = XMMatrixPerspectiveFovLH(m_fTargetFovAngleY, m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar);
	}
	else
	{
		_float fAcc(0.f);
		CEvent_Handler::Get_Instance()->Register_OnTickListener(shared_from_this(),
			[=](_float _fTimeDelta) mutable->_bool
			{
				if (fAcc < 1.f)
				{
					fAcc += _fTimeDelta / _fDuration;
					m_mProjection = XMMatrixPerspectiveFovLH(
						Function::Lerp(m_tCameraDesc.fFovAngleY, m_fTargetFovAngleY, fAcc, _fWeight), m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar);
				}

				return !(fAcc >= 1.f);
			}
		);
	}
}

void CCamera::Release_FOV(_float _fDuration, _float _fWeight)
{
	if (TYPE::ORTHOGONAL == m_tCameraDesc.eType)
	{
		MSG_RETURN(, "CCamera::Adjust_FOV", "Orthogonal Camera does not support Release_FOV");
	}

	if (!_fDuration)
	{
		m_mProjection = XMMatrixPerspectiveFovLH(m_tCameraDesc.fFovAngleY, m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar);
	}
	else
	{
		_float fAcc(0.f);
		CEvent_Handler::Get_Instance()->Register_OnTickListener(shared_from_this(),
			[=](_float _fTimeDelta) mutable->_bool
			{
				if (fAcc < 1.f)
				{
					fAcc += _fTimeDelta / _fDuration;
					m_mProjection = XMMatrixPerspectiveFovLH(
						Function::Lerp(m_fTargetFovAngleY, m_tCameraDesc.fFovAngleY, fAcc, _fWeight), m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar);
				}

				return !(fAcc >= 1.f);
			}
		);
	}
}
