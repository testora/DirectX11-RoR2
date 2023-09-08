#include "ClientPCH.h"
#include "RailGunner_Bullet.h"

CRailGunner_Bullet::CRailGunner_Bullet(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CRailGunner_Bullet::CRailGunner_Bullet(const CRailGunner_Bullet& _rhs)
    : CGameObject   (_rhs)
    , m_pTexBullet  (_rhs.m_pTexBullet)
{
}

HRESULT CRailGunner_Bullet::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRailGunner_Bullet::Initialize(any)
{
    return S_OK;
}

void CRailGunner_Bullet::Tick(_float _fTimeDelta)
{
}

void CRailGunner_Bullet::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CRailGunner_Bullet::Render()
{
    return S_OK;
}

HRESULT CRailGunner_Bullet::Fetch(any)
{
    return S_OK;
}

void CRailGunner_Bullet::Shoot()
{
}
