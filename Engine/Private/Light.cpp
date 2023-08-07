#include "EnginePCH.h"
#include "Light.h"

CLight::CLight(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
}

HRESULT CLight::Initialize(const LIGHTDESC&)
{
	return S_OK;
}

shared_ptr<CLight> CLight::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const LIGHTDESC& _tLightDesc)
{
	shared_ptr<CLight> pInstance = make_private_shared(CLight, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_tLightDesc)))
	{
		MSG_RETURN(nullptr, "CLight::Create", "Failed to Initialize");
	}

	return pInstance;
}
