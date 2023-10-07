#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator final : public CBehavior
{
private:
	explicit CAnimator();
	explicit CAnimator(const CAnimator&);
	virtual ~CAnimator() DEFAULT;

public:
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	template<typename T>
	_bool							Is_Playing(T eAnimation) const;
	template<typename T>
	_bool							Is_Finished(T eAnimation) const;
	template<typename T>
	_float							Get_Duration(T eAnimation) const;

public:
	template<typename T>
	void							Set_Default(T eAnimation);
	template<typename T>
	void							Play_Animation(T eAnimation, _float fPlaySpeed = 1.f, _bool bReverse = false, _float fInterpolationDuration = g_fDefaultInterpolationDuration, _bool bLoop = true);
	template<typename T>
	void							Fix_Animation(T eAnimation, _float fRatio);
	template<typename T>
	void							Blend_Animation(T eAnimation, _float fRatio);

private:
	_bool							Is_Playing(_uint iAnimationIndex) const;
	_bool							Is_Finished(_uint iAnimationIndex) const;
	_float							Get_Duration(_uint iAnimationIndex) const;

	void							Set_DefaultAnimation(_uint iAnimationIndex);
	void							Set_Animation(_uint iAnimationIndex, _float fPlaySpeed, _bool bReverse, _float fInterpolationDuration, _bool bLoop);
	void							Fix_Animation(_uint iAnimationIndex, _float fRatio);
	void							Blend_Animation(_uint iAnimationIndex, _float fRatio);

private:
	shared_ptr<class CModel>		m_pTargetModel;

public:
	static shared_ptr<CAnimator>	Create(shared_ptr<class CGameObject> pOwner);
	virtual shared_ptr<CBehavior>	Clone(any = g_aNull) override;
};

#include "Animator.inl"

END
