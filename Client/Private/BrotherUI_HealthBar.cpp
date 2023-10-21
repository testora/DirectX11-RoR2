#include "ClientPCH.h"
#include "BrotherUI_HealthBar.h"
#include "GameInstance.h"

CBrotherUI_HealthBar::CBrotherUI_HealthBar(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

HRESULT CBrotherUI_HealthBar::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CBrotherUI_HealthBar::Initialize(any)
{
    return E_NOTIMPL;
}

void CBrotherUI_HealthBar::Tick(_float fTimeDelta)
{
}

void CBrotherUI_HealthBar::Late_Tick(_float fTimeDelta)
{
}

HRESULT CBrotherUI_HealthBar::Render()
{
    return S_OK;
}

HRESULT CBrotherUI_HealthBar::Ready_Components()
{
    return S_OK;
}

HRESULT CBrotherUI_HealthBar::Ready_Behaviors()
{
    return S_OK;
}

shared_ptr<CBrotherUI_HealthBar> CBrotherUI_HealthBar::Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>)
{
    return shared_ptr<CBrotherUI_HealthBar>();
}

shared_ptr<CGameObject> CBrotherUI_HealthBar::Clone(any)
{
    return shared_ptr<CGameObject>();
}
