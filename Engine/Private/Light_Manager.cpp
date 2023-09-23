#include "EnginePCH.h"
#include "Light_Manager.h"
#include "Light.h"
#include "Shader.h"

HRESULT CLight_Manager::Reserve_Manager(const SCENE _eSceneMax)
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
				(*iter)->Tick();
				++iter;
			}
		}
	}
}

HRESULT CLight_Manager::Add_Light(const SCENE _eScene, LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Invalid Range: SCENE");
	}

	if (m_iLightCount >= g_iMaxLights - 2)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Out of Range: m_iLightCount");
	}

	shared_ptr<CLight> pLight = CLight::Create(_tLightDesc, _pTransform);

	if (nullptr == pLight)
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Add_Light", "Failed to Create: CLight");
	}

	m_arrLights[IDX(_eScene)].emplace_back(pLight);

	++m_iLightCount;

    return S_OK;
}

HRESULT CLight_Manager::Clear_Lights(const SCENE _eScene)
{
	if (!Function::InRange(_eScene, static_cast<SCENE>(0), m_eSceneMax))
	{
		MSG_RETURN(E_FAIL, "CLight_Manager::Clear_Lights", "Invalid Range: SCENE");
	}

	m_iLightCount -= static_cast<_uint>(m_arrLights[IDX(_eScene)].size());

	m_arrLights[IDX(_eScene)].clear();

	return S_OK;
}

HRESULT CLight_Manager::Bind_Lights(shared_ptr<CShader> _pShader, shared_ptr<CVIBuffer_Rect> _pVIBuffer)
{
	HRESULT hr = S_OK;

	for (size_t i = 0; i < IDX(m_eSceneMax); ++i)
	{
		for (auto& pLight : m_arrLights[i])
		{
			if (FAILED(pLight->Bind_Light(_pShader, _pVIBuffer)))
			{
				hr = E_FAIL;
				MSG_BOX("CLight_Manager::Bind_Lights", "Failed to Bind_Light");
			}
		}
	}

    return hr;
}
