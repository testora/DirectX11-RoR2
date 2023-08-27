#include "EnginePCH.h"
#include "Animator.h"
#include "Model.h"
#include "GameObject.h"

CAnimator::CAnimator()
	: CBehavior(BEHAVIOR::ANIMATOR)
{
}

CAnimator::CAnimator(const CAnimator& _rhs)
	: CBehavior(_rhs)
{
}

HRESULT CAnimator::Initialize(shared_ptr<class CGameObject> _pOwner)
{
	if (FAILED(__super::Initialize(_pOwner)))
	{
		MSG_RETURN(E_FAIL, "CAnimator::Initialize", "Failed to __super::Initialize");
	}

	if (m_pOwner.expired())
	{
		MSG_RETURN(E_FAIL, "CAnimator::Initialize", "weak_ptr Expired: m_pOwner");
	}

	shared_ptr<CModel> pTargetModel = m_pOwner.lock()->Get_Component<CModel>(COMPONENT::MODEL);
	if (nullptr == pTargetModel)
	{
		MSG_RETURN(E_FAIL, "CAnimator::Initialize", "Failed to Get_Component");
	}

	m_pTargetModel = pTargetModel;

	return S_OK;
}

void CAnimator::Tick(_float _fTimeDelta)
{
	m_pTargetModel->Tick_Animation(_fTimeDelta);
}

void CAnimator::Late_Tick(_float _fTimeDelta)
{
}

void CAnimator::Play_Animation(_uint _iAnimIdx, _float _fInterpolationDuration, _bool _bLoop)
{
	m_pTargetModel->Set_Animation(_iAnimIdx, _fInterpolationDuration, _bLoop);
}

shared_ptr<CAnimator> CAnimator::Create(shared_ptr<class CGameObject> _pOwner)
{
	shared_ptr<CAnimator> pInstance = make_private_shared(CAnimator);

	if (FAILED(pInstance->Initialize(_pOwner)))
	{
		MSG_RETURN(nullptr, "CAnimator::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBehavior> CAnimator::Clone(any)
{
	return make_private_shared_copy(CAnimator, *this);
}
