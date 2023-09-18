#pragma once
#include "Client_Define.h"
#include "Hostile.h"
#include "System.h"

BEGIN(Engine)
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CGolem_Behavior final : public CHostile
{
public:
	enum class STATE	{ UNTARGET, TARGET, HIT, MAX };

private:
	explicit CGolem_Behavior() DEFAULT;
	explicit CGolem_Behavior(const CGolem_Behavior&) DEFAULT;
	virtual ~CGolem_Behavior() DEFAULT;

public:
	virtual HRESULT												Initialize(shared_ptr<class CGameObject> pOwner, shared_ptr<class CGameObject> pTarget, const ENTITYDESC*) override;
	virtual void												Tick(_float fTimeDelta) override;
	virtual void												Late_Tick(_float fTimeDelta) override;

private:
	HRESULT														Ready_State();

public:
	void														Set_State(STATE eState);

private:
	void														Manage_State();
	void														Handle_State(_float fTimeDelta);

private:
	STATE														m_eState	= STATE::MAX;
	unordered_map<STATE, shared_ptr<class Engine::ISystem>>		m_umapState;

	shared_ptr<class CGolem>									m_pGolem;

	shared_ptr<CAnimator>										m_pAnimator;

	_float														m_fTimeAcc	= 0.f;
	_float														m_fAtkAcc	= 3.f;

public:
	static shared_ptr<CGolem_Behavior>							Create(shared_ptr<class CGameObject> pOwner, shared_ptr<class CGameObject> pTarget, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>								Clone(any = g_aNull) override;
};

END
