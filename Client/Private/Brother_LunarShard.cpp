#include "ClientPCH.h"
#include "Brother_LunarShard.h"

CBrother_LunarShard::CBrother_LunarShard(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
    : CGameObject(_pDevice, _pContext)
{
}

CBrother_LunarShard::CBrother_LunarShard(const CBrother_LunarShard& _rhs)
    : CGameObject(_rhs)
{
}

HRESULT CBrother_LunarShard::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CBrother_LunarShard::Initialize(any)
{
    return E_NOTIMPL;
}

void CBrother_LunarShard::Tick(_float fTimeDelta)
{
}

void CBrother_LunarShard::Late_Tick(_float fTimeDelta)
{
}

HRESULT CBrother_LunarShard::Render()
{
    return E_NOTIMPL;
}

HRESULT CBrother_LunarShard::Fetch(any aPoolPosition)
{
    return E_NOTIMPL;
}

HRESULT CBrother_LunarShard::Ready_Components()
{
    return E_NOTIMPL;
}

HRESULT CBrother_LunarShard::Ready_Behaviors()
{
    return E_NOTIMPL;
}

void CBrother_LunarShard::Set_Target(shared_ptr<class CMonster>)
{
}

void CBrother_LunarShard::Manage_State(_float fTimeDelta)
{
}

void CBrother_LunarShard::Search_Target()
{
}

void CBrother_LunarShard::Destroy()
{
}

shared_ptr<CBrother_LunarShard> CBrother_LunarShard::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
    return shared_ptr<CBrother_LunarShard>();
}

shared_ptr<CGameObject> CBrother_LunarShard::Clone(any)
{
    return shared_ptr<CGameObject>();
}
