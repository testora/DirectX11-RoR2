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

	if (m_pTargetModel->Is_AnimationFinished())
	{
		m_pTargetModel->Set_Animation(m_iReservedAnimationIndex, m_fReservedInterpolationDuration, m_bReservedLoop);
	}
}

void CAnimator::Late_Tick(_float _fTimeDelta)
{
}

_uint CAnimator::Get_Animation() const
{
	return m_pTargetModel->Get_AnimationIndex();
}

void CAnimator::Set_Animation(_uint _iAnimationIndex, _float _fInterpolationDuration, _bool _bLoop)
{
	m_pTargetModel->Set_Animation(IDX(_iAnimationIndex), _fInterpolationDuration, _bLoop);
}

void CAnimator::Set_Animation(_uint _iAnimationIndex, _uint _iNextAnimationIndex, _float _fInterpolationDuration, _float _fReservedInterpolationDuration, _bool _bReservedLoop)
{
	m_pTargetModel->Set_Animation(IDX(_iAnimationIndex), _fInterpolationDuration, false);

	m_bReservedLoop						= _bReservedLoop;
	m_fReservedInterpolationDuration	= _fReservedInterpolationDuration;
	m_iReservedAnimationIndex			= _iNextAnimationIndex;
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
