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
	void							Play_Animation(_uint iAnimIdx, _float fInterpolationDuration = g_fDefaultInterpolationDuration, _bool bLoop = true);

private:
	shared_ptr<class CModel>		m_pTargetModel;

public:
	static shared_ptr<CAnimator>	Create(shared_ptr<class CGameObject> pOwner);
	virtual shared_ptr<CBehavior>	Clone(any = any()) override;
};

END
