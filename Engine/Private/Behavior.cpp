#include "EnginePCH.h"
#include "Behavior.h"

CBehavior::CBehavior(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const BEHAVIOR _eType)
	: m_pDevice	(_pDevice)
	, m_pContext(_pContext)
	, m_eType	(_eType)
{
}

CBehavior::CBehavior(const CBehavior& _rhs)
	: m_pDevice	(_rhs.m_pDevice)
	, m_pContext(_rhs.m_pContext)
	, m_eType	(_rhs.m_eType)
{
}

HRESULT CBehavior::Initialize(std::any)
{
	return E_NOTIMPL;
}

void CBehavior::Tick(_float fTimeDelta)
{
}

void CBehavior::Late_Tick(_float fTimeDelta)
{
}
