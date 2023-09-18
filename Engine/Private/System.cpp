#include "EnginePCH.h"
#include "System.h"

void ISystem::Tick(_float _fTimeDelta)
{
}

void ISystem::Late_Tick(_float _fTimeDelta)
{
}

HRESULT ISystem::Render()
{
	return S_OK;
}
