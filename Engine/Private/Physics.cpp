#include "EnginePCH.h"
#include "Physics.h"

CPhysics::CPhysics(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CBehavior(_pDevice, _pContext, BEHAVIOR::MAX)
{
}

CPhysics::CPhysics(const CPhysics& _rhs)
	: CBehavior(_rhs)
{
}

HRESULT CPhysics::Initialize(any _arg)
{
	return S_OK;
}

void CPhysics::Tick(_float _fTimeDelta)
{
}

void CPhysics::Late_Tick(_float _fTimeDelta)
{
}

shared_ptr<CPhysics> CPhysics::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
{
	return make_private_shared(CPhysics, _pDevice, _pContext);
}

shared_ptr<CBehavior> CPhysics::Clone(any _arg)
{
	return make_private_shared_copy(CPhysics, *this);
}
