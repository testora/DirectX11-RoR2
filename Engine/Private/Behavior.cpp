#include "EnginePCH.h"
#include "Behavior.h"

CBehavior::CBehavior(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const BEHAVIOR _eType)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
	, m_eType(_eType)
{
}
