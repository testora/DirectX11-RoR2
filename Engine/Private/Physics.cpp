#include "EnginePCH.h"
#include "Physics.h"

CPhysics::CPhysics(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CBehavior(_pDevice, _pContext, BEHAVIOR::MAX)
{
}
