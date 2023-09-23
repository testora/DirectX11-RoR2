#include "EnginePCH.h"
#include "Light.h"
#include "Transform.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

HRESULT CLight::Initialize(const LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	switch (m_tLightDesc.eLightType)
	{
	case LIGHTDESC::LIGHTTYPE::DIRECTIONAL:
		break;

	case LIGHTDESC::LIGHTTYPE::POINT:
		if (nullptr == _pTransform)
		{
			MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: shared_ptr<CTransform>");
		}
		break;

	default:
		MSG_RETURN(E_FAIL, "CLight::Initialize", "Invalid Parameter: LIGHTDESC::LIGHTTYPE");
	}

	m_tLightDesc = _tLightDesc;
	m_pTransform = _pTransform;

	return S_OK;
}

void CLight::Tick()
{
	if (m_pTransform.expired())
	{
		return;
	}

	m_tLightDesc.vPosition = m_pTransform.lock()->Get_State(TRANSFORM::POSITION);
}

_bool CLight::Is_Expired() const
{
	switch (m_tLightDesc.eLightType)
	{
	case LIGHTDESC::LIGHTTYPE::DIRECTIONAL:
		return false;

	case LIGHTDESC::LIGHTTYPE::POINT:
		return m_pTransform.expired();

	default:
		MSG_RETURN(true, "CLight::Is_Expired", "Invalid LightType");
	}
}

HRESULT CLight::Bind_Light(shared_ptr<CShader> _pShader, shared_ptr<CVIBuffer_Rect> _pVIBuffer)
{
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
	case LIGHTDESC::LIGHTTYPE::DIRECTIONAL:
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
	case LIGHTDESC::LIGHTTYPE::POINT:
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

shared_ptr<CLight> CLight::Create(const LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	shared_ptr<CLight> pInstance = make_private_shared(CLight);

	if (FAILED(pInstance->Initialize(_tLightDesc, _pTransform)))
	{
		MSG_RETURN(nullptr, "CLight::Create", "Failed to Initialize");
	}

	return pInstance;
}
