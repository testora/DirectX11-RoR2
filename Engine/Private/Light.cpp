#include "EnginePCH.h"
#include "Light.h"
#include "Transform.h"

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
		MSG_RETURN(true, "CLight::Is_Expired", "Invalid Parameter: LIGHTDESC::LIGHTTYPE");
	}
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
