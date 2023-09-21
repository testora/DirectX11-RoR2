#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Engine)
class CTransform;
class CModel;
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CBrother_Phase abstract : public ISystem
{
protected:
	explicit CBrother_Phase() DEFAULT;
	virtual ~CBrother_Phase() DEFAULT;

public:
	virtual HRESULT						Initialize(shared_ptr<class CGameObject> pOwner, shared_ptr<class CBrother_Behavior>, const ENTITYDESC*);
	virtual HRESULT						Initialize(shared_ptr<CBrother_Phase> pPrevious);
	virtual void						Tick(_float fTimeDelta) override		PURE;
	virtual void						Late_Tick(_float fTimeDelta) override	PURE;

protected:
	_bool								Is_Target_InRange(_float fRange) const;

	void								Change_Phase(BROTHER_PHASE);

	void								Dash(DIRECTION eRandom = DIRECTION::MAX);
	void								Smash();
	void								Sprint_Smash();

private:
	virtual void						Handle_State(_float fTimeDelta)			PURE;

protected:
	_float								m_fPhaseTimerAcc	= 0.f;
	_float								m_fStateTimerAcc	= 0.f;

	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CModel>					m_pModel;
	shared_ptr<CPhysics>				m_pPhysics;
	shared_ptr<CAnimator>				m_pAnimator;

	shared_ptr<class CGameObject>		m_pTarget;
	shared_ptr<CTransform>				m_pTargetTransform;

	const ENTITYDESC*					m_pEntityDesc;

private:
	weak_ptr<class CBrother_Behavior>	m_pBehavior;
};

END
