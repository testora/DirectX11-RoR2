#include "EnginePCH.h"
#include "Light.h"
#include "Transform.h"

HRESULT CLight::Initialize(const LIGHTDESC _tLightDesc, shared_ptr<CTransform> _pTransform)
{
	m_tLightDesc = _tLightDesc;
	m_pTransform = _pTransform;

	return S_OK;
}

_bool CLight::Is_Expired() const
{
	return m_pTransform.expired();
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
