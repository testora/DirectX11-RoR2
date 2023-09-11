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
	T								Current_Animation() const;

public:
	template<typename T>
	void							Play_Animation(T eAnimation, _float fPlaySpeed = 1.f, _bool bReverse = false, _float fInterpolationDuration = g_fDefaultInterpolationDuration, _bool bLoop = true);

private:
	_uint							Get_Animation() const;
	void							Set_Animation(_uint iAnimationIndex, _float fPlaySpeed, _bool bReverse, _float fInterpolationDuration, _bool bLoop);

private:
	shared_ptr<class CModel>		m_pTargetModel;

public:
	static shared_ptr<CAnimator>	Create(shared_ptr<class CGameObject> pOwner);
	virtual shared_ptr<CBehavior>	Clone(any = g_aNull) override;
};

#include "Animator.inl"

END
