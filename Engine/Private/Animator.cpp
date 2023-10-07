#include "EnginePCH.h"
#include "Animator.h"
#include "Animation.h"
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

_bool CAnimator::Is_Playing(_uint _iAnimationIndex) const
{
	return m_pTargetModel->Is_AnimationPlaying(_iAnimationIndex);
}

_bool CAnimator::Is_Finished(_uint _iAnimationIndex) const
{
	return m_pTargetModel->Is_AnimationFinished(_iAnimationIndex);
}

_float CAnimator::Get_Duration(_uint iAnimationIndex) const
{
	return m_pTargetModel->Get_Animation(iAnimationIndex)->Get_Duration();
}

void CAnimator::Set_DefaultAnimation(_uint _iAnimationIndex)
{
	m_pTargetModel->Set_DefaultAnimation(_iAnimationIndex);
}

void CAnimator::Set_Animation(_uint _iAnimationIndex, _float _fPlaySpeed, _bool _bReverse, _float _fInterpolationDuration, _bool _bLoop)
{
	m_pTargetModel->Set_Animation(IDX(_iAnimationIndex), _fPlaySpeed, _bReverse, _fInterpolationDuration, _bLoop);
	if (false == _bLoop)
	{
		m_pTargetModel->Reset_Animation(_iAnimationIndex);
	}
}

void CAnimator::Fix_Animation(_uint _iAnimationIndex, _float _fRatio)
{
	m_pTargetModel->Fix_Animation(_iAnimationIndex, _fRatio);
}

void CAnimator::Blend_Animation(_uint _iAnimationIndex, _float _fRatio)
{
	m_pTargetModel->Blend_Animation(_iAnimationIndex, _fRatio);
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
