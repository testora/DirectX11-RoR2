#include "EnginePCH.h"
#include "Light.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "GameInstance.h"
#include "Camera.h"

HRESULT CLight::Initialize(const LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	switch (_tLightDesc.eLightType)
	{
	case LIGHTTYPE::DIRECTIONAL:
		break;

	case LIGHTTYPE::POINT:
		if (nullptr == _pTransform)
		{
			MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: shared_ptr<CTransform>");
		}
		break;

	case LIGHTTYPE::SHADOW:
	{
		m_mShadowView = XMMatrixLookToLH(
			_float4(_tLightDesc.vPosition, 1.f) - _float4(_tLightDesc.vDirection, 0.f).normalize() * _tLightDesc.fRange,
			_float4(_tLightDesc.vDirection, 0.f).normalize(),
			_float4(0.f, 1.f, 0.f, 0.f));

		switch (_tLightDesc.eShadowType)
		{
		case SHADOWTYPE::DIRECTIONAL:
		{
		//	GRAPHICDESC				tGraphicDesc	= CGameInstance::Get_Instance()->Get_GraphicDesc();
			CCamera::CAMERA_DESC	tCameraDesc		= CPipeLine::Get_Instance()->Get_Camera()->Get_Desc();
		//	m_mShadowProjection = XMMatrixOrthographicLH(
		//		static_cast<_float>(tGraphicDesc.iWinCX), static_cast<_float>(tGraphicDesc.iWinCY), tCameraDesc.fNear, tCameraDesc.fFar);
			m_mShadowProjection = XMMatrixOrthographicLH(
				static_cast<_float>(10), static_cast<_float>(10), tCameraDesc.fNear, tCameraDesc.fFar);
		}
		break;

		case SHADOWTYPE::DYNAMIC:
		{
			if (nullptr == _pTransform)
			{
				MSG_RETURN(E_FAIL, "CLight::Initialize", "Nullptr Exception");
			}
			m_mShadowProjection = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);
		}
		break;

		case SHADOWTYPE::STATIC:
		{
		//	CCamera::CAMERA_DESC	tCameraDesc = CPipeLine::Get_Instance()->Get_Camera()->Get_Desc();
		//	m_mShadowProjection = XMMatrixOrthographicLH(
		//		static_cast<_float>(10), static_cast<_float>(10), tCameraDesc.fNear, tCameraDesc.fFar);
			m_mShadowProjection = CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION);
		}
		break;

		default:
			MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: SHADOWTYPE");
		}
	}
	break;

	default:
		MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: LIGHTTYPE");
	}

	m_tLightDesc = _tLightDesc;
	m_pTransform = _pTransform;

	m_tLightDesc.vDirection = _float3(_tLightDesc.vDirection).normalize();

	return S_OK;
}

void CLight::Tick()
{
	if (m_pTransform.expired())
	{
		return;
	}

	shared_ptr<CTransform> pTransform = m_pTransform.lock();

	switch (m_tLightDesc.eLightType)
	{
	case LIGHTTYPE::POINT:
		m_tLightDesc.vPosition = pTransform->Get_State(TRANSFORM::POSITION);
		break;

	case LIGHTTYPE::SHADOW:
	{
		switch (m_tLightDesc.eShadowType)
		{
		case SHADOWTYPE::DIRECTIONAL:
		{
			m_mShadowView = XMMatrixLookToLH(
				_float4(pTransform->Get_State(TRANSFORM::POSITION) - _float3(m_tLightDesc.vDirection) * m_tLightDesc.fRange, 1.f),
				_float4(m_tLightDesc.vDirection, 0.f),
				XMVectorSet(0.f, 1.f, 0.f, 0.f));
		}
		break;
		case SHADOWTYPE::DYNAMIC:
		{
			m_mShadowView = XMMatrixLookToLH(
				pTransform->Get_State(TRANSFORM::POSITION),
				pTransform->Get_State(TRANSFORM::LOOK),
				XMVectorSet(0.f, 1.f, 0.f, 0.f));
		}
		break;
		}
	}
	break;
	}
}

_bool CLight::Is_Expired() const
{
	switch (m_tLightDesc.eLightType)
	{
	case LIGHTTYPE::DIRECTIONAL:
		return false;

	case LIGHTTYPE::SHADOW:
		return m_tLightDesc.eShadowType == SHADOWTYPE::DYNAMIC && m_pTransform.expired();

	case LIGHTTYPE::POINT:
		return m_pTransform.expired();

	default:
		MSG_RETURN(true, "CLight::Is_Expired", "Invalid LightType");
	}
}

HRESULT CLight::Bind_Light(shared_ptr<CShader> _pShader, shared_ptr<CVIBuffer_Rect> _pVIBuffer)
{
	if (LIGHTTYPE::SHADOW == m_tLightDesc.eLightType)
	{
		return S_OK;
	}

	if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTDIF, m_tLightDesc.vDiffuse)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTDIF");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTAMB, m_tLightDesc.vAmbient)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTAMB");
	}
	if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTSPC,m_tLightDesc.vSpecular)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTSPC");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_ATT0, m_tLightDesc.fAttenuation0)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_ATT0");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_ATT1, m_tLightDesc.fAttenuation1)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_ATT1");
	}
	if (FAILED(_pShader->Bind_Float(SHADER_ATT2, m_tLightDesc.fAttenuation2)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_ATT2");
	}

	switch (m_tLightDesc.eLightType)
	{
	case LIGHTTYPE::DIRECTIONAL:
	{
		if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTDIR, _float4(m_tLightDesc.vDirection, 0.f))))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTDIR");
		}

		if (FAILED(_pShader->BeginPass(0)))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to BeginPass");
		}
		if (FAILED(_pVIBuffer->Render()))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Render");
		}
	}
	break;
	case LIGHTTYPE::POINT:
	{
		if (FAILED(_pShader->Bind_Vector(SHADER_LIGHTPOS, _float4(m_tLightDesc.vPosition, 1.f))))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Vector: SHADER_LIGHTPOS");
		}
		if (FAILED(_pShader->Bind_Float(SHADER_LIGHTRNG, m_tLightDesc.fRange)))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Bind_Float: SHADER_LIGHTRNG");
		}

		if (FAILED(_pShader->BeginPass(1)))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to BeginPass");
		}
		if (FAILED(_pVIBuffer->Render()))
		{
			MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Failed to Render");
		}
	}
	break;

	default:
		MSG_RETURN(E_FAIL, "CLight::Bind_Light", "Invalid LightType");
	}

	return S_OK;
}

HRESULT CLight::Bind_ShadowMatrices(shared_ptr<CShader> _pShader)
{
	if (FAILED(_pShader->Bind_Matrix(SHADER_MATVIEW, m_mShadowView)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_ShadowMatrices", "Failed to Bind_Matrix: SHADER_MATVIEW");
	}

	if (FAILED(_pShader->Bind_Matrix(SHADER_MATPROJ, m_mShadowProjection)))
	{
		MSG_RETURN(E_FAIL, "CLight::Bind_ShadowMatrices", "Failed to Bind_Matrix: SHADER_MATPROJ");
	}

	return S_OK;
}

shared_ptr<CLight> CLight::Create(const LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	shared_ptr<CLight> pInstance = make_private_shared(CLight);

	if (FAILED(pInstance->Initialize(_tLightDesc, _pTransform)))
	{
		MSG_RETURN(nullptr, "CLight::Create", "Failed to Initialize");
	}

	return pInstance;
}
