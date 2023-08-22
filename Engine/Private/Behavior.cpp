#include "EnginePCH.h"
#include "Behavior.h"
#include "GameObject.h"

CBehavior::CBehavior(const BEHAVIOR _eType)
	: m_eType(_eType)
{
}

CBehavior::CBehavior(const CBehavior& _rhs)
	: m_eType(_rhs.m_eType)
{
}

HRESULT CBehavior::Initialize(any _pOwner)
{
	if (!_pOwner.has_value())
	{
		MSG_RETURN(E_FAIL, "CBehavior::Initialize", "Invalid Parameter");
	}

	shared_ptr<CGameObject> pOwner = any_cast<shared_ptr<CGameObject>>(_pOwner);
	if (nullptr == pOwner)
	{
		MSG_RETURN(E_FAIL, "CBehavior::Initialize", "Failed to any_cast");
	}

	m_pOwner = pOwner;

	return S_OK;
}

void CBehavior::Tick(_float _fTimeDelta)
{
}

void CBehavior::Late_Tick(_float _fTimeDelta)
{
}
