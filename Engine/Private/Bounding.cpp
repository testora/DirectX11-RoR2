#include "EnginePCH.h"
#include "Bounding.h"

HRESULT CBounding::Initialize(any _pColliderDesc)
{
	m_pColliderDesc = any_cast<COLLIDERDESC*>(_pColliderDesc);

	return S_OK;
}
