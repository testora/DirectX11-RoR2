#include "ClientPCH.h"
#include "RailGunner_PistolBullet.h"

CRailGunner_PistolBullet::CRailGunner_PistolBullet(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CRailGunner_Bullet(_pDevice, _pContext)
{
}

CRailGunner_PistolBullet::CRailGunner_PistolBullet(const CRailGunner_PistolBullet& _rhs)
    : CRailGunner_Bullet(_rhs)
{
}

HRESULT CRailGunner_PistolBullet::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRailGunner_PistolBullet::Initialize(any)
{
    return S_OK;
}

void CRailGunner_PistolBullet::Tick(_float _fTimeDelta)
{
}

void CRailGunner_PistolBullet::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CRailGunner_PistolBullet::Render()
{
    return S_OK;
}

HRESULT CRailGunner_PistolBullet::Fetch(any)
{
    return S_OK;
}

shared_ptr<CRailGunner_PistolBullet> CRailGunner_PistolBullet::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    shared_ptr<CRailGunner_PistolBullet> pInstance = make_private_shared(CRailGunner_PistolBullet, _pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_RETURN(nullptr, "CRailGunner_PistolBullet::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CGameObject> CRailGunner_PistolBullet::Clone(any)
{
    shared_ptr<CRailGunner_PistolBullet> pInstance = make_private_shared_copy(CRailGunner_PistolBullet, *this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_RETURN(nullptr, "CRailGunner_PistolBullet::Clone", "Failed to Initialize");
    }

    return pInstance;
}
