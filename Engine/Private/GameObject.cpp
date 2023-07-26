#include "EnginePCH.h"
#include "GameObject.h"

CGameObject::CGameObject(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: m_pDevice(_pDevice), m_pContext(_pContext)
{

}
