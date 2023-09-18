#include "EnginePCH.h"
#include "Component.h"

CComponent::CComponent(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const COMPONENT _eType)
	: m_pDevice	(_pDevice)
	, m_pContext(_pContext)
	, m_eType	(_eType)
{
}

CComponent::CComponent(const CComponent& _rhs)
	: m_pDevice	(_rhs.m_pDevice)
	, m_pContext(_rhs.m_pContext)
	, m_eType	(_rhs.m_eType)
{
}

shared_ptr<CComponent> CComponent::shared_from_componenet()
{
	return static_pointer_cast<CComponent>(shared_from_this());
}
