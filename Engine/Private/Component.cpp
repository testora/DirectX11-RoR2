#include "EnginePCH.h"
#include "Component.h"

CComponent::CComponent(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const COMPONENT _eType)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
	, m_eType(_eType)
{
}
