#include "EnginePCH.h"
#include "Light_Manager.h"
#include "Light.h"
#include "Shader.h"

HRESULT CLight_Manager::Reserve_Lights(const SCENE _eSceneMax)
{
	m_arrLights	= Function::CreateDynamicArray<Lights>(IDX(_eSceneMax), false);

	m_eSceneMax	= _eSceneMax;

	return S_OK;
}

void CLight_Manager::Tick()
{
	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		for (auto iter = m_arrLights[i].begin(); iter != m_arrLights[i].end();)
		{
			if ((*iter)->Is_Expired())
			{
				iter = m_arrLights[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
}

void CLight_Manager::Late_Tick()
{
	size_t i = 0;
	for (size_t j = 0; j < IDX(m_eSceneMax); ++j)
	{
		for (size_t k = 0; k < m_arrLights[j].size(); ++i, ++k)
		{
			m_arrLightType[i]		= m_arrLights[j][k]->m_tLightDesc.eLightType;
			m_arrPosition[i]		= _float4(m_arrLights[j][k]->m_tLightDesc.vPosition, 1.f);
			m_arrDirection[i]		= _float4(m_arrLights[j][k]->m_tLightDesc.vDirection, 0.f);
			m_arrDiffuse[i]			= m_arrLights[j][k]->m_tLightDesc.vDiffuse;
			m_arrSpecular[i]		= m_arrLights[j][k]->m_tLightDesc.vSpecular;
			m_arrAmbient[i]			= m_arrLights[j][k]->m_tLightDesc.vAmbient;
			m_arrRange[i]			= m_arrLights[j][k]->m_tLightDesc.fRange;
			m_arrAttenuation0[i]	= m_arrLights[j][k]->m_tLightDesc.fAttenuation0;
			m_arrAttenuation1[i]	= m_arrLights[j][k]->m_tLightDesc.fAttenuation1;
			m_arrAttenuation2[i]	= m_arrLights[j][k]->m_tLightDesc.fAttenuation2;
		}
	}

	if (FAILED(Bind_Lights()))
	{
		MSG_RETURN(, "CLight_Manager::Late_Tick", "Failed to Bind_Lights");
	}
}

HRESULT CLight_Manager::Add_Shaders(shared_ptr<class CShader> _pShader)
{
	if (nullptr == _pShader)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Shaders", "Nullptr Exception");
	}

	m_vecShaders.emplace_back(_pShader);

	return S_OK;
}

HRESULT CLight_Manager::Add_Lights(const SCENE _eScene, LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	if (nullptr == _pTransform)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Lights", "Nullptr Exception");
	}

	if (m_iLightCount - 1 < g_iMaxLights)
	{
		++m_iLightCount;
	}
	else
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Lights", "Out of Range: m_iLightCount");
	}

	m_arrLights[IDX(_eScene)].emplace_back(CLight::Create(_tLightDesc, _pTransform));

    return S_OK;
}

HRESULT CLight_Manager::Clear_Lights(const SCENE _eScene)
{
	m_iLightCount -= static_cast<_uint>(m_arrLights[IDX(_eScene)].size());

	m_arrLights[IDX(_eScene)].clear();

	return S_OK;
}

HRESULT CLight_Manager::Bind_Lights()
{
	for (auto& pShader : m_vecShaders)
	{
		if (FAILED(pShader->Bind_RawValue(SHADER_LIGHTCNT, &m_iLightCount, sizeof m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_RawValue:SHADER_LIGHTCNT");
		}
		if (FAILED(pShader->Bind_RawValue(SHADER_LIGHTTYPE, m_arrLightType.data(), sizeof(int) * m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_RawValue:SHADER_LIGHTTYPE");
		}
		if (FAILED(pShader->Bind_VectorArray(SHADER_LIGHTPOS, m_arrPosition.data(), m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_VectorArray:SHADER_LIGHTPOS");
		}
		if (FAILED(pShader->Bind_VectorArray(SHADER_LIGHTDIR, m_arrDirection.data(), m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_VectorArray:SHADER_LIGHTDIR");
		}
		if (FAILED(pShader->Bind_VectorArray(SHADER_LIGHTDIF, m_arrDiffuse.data(), m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_VectorArray:SHADER_LIGHTDIF");
		}
		if (FAILED(pShader->Bind_VectorArray(SHADER_LIGHTSPC, m_arrSpecular.data(), m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_VectorArray:SHADER_LIGHTSPC");
		}
		if (FAILED(pShader->Bind_VectorArray(SHADER_LIGHTAMB, m_arrAmbient.data(), m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_VectorArray:SHADER_LIGHTAMB");
		}
		if (FAILED(pShader->Bind_RawValue(SHADER_LIGHTRNG, m_arrRange.data(), sizeof(float) * m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_RawValue:SHADER_LIGHTRNG");
		}
		if (FAILED(pShader->Bind_RawValue(SHADER_ATT0, m_arrAttenuation0.data(), sizeof(float) * m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_RawValue:SHADER_ATT0");
		}
		if (FAILED(pShader->Bind_RawValue(SHADER_ATT1, m_arrAttenuation1.data(), sizeof(float) * m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_RawValue:SHADER_ATT1");
		}
		if (FAILED(pShader->Bind_RawValue(SHADER_ATT2, m_arrAttenuation2.data(), sizeof(float) * m_iLightCount)))
		{
			MSG_RETURN(E_FAIL, "CLight_Manager::Bind_Lights", "Failed to Bind_RawValue:SHADER_ATT2");
		}
	}

    return S_OK;
}
