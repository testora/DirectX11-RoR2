#pragma once
#include "Client_Define.h"
#include "Hostile.h"

BEGIN(Client)

class CBrother_Behavior final : public CHostile
{
private:
	explicit CBrother_Behavior() DEFAULT;
	explicit CBrother_Behavior(const CBrother_Behavior&) DEFAULT;
	virtual ~CBrother_Behavior() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, shared_ptr<class CGameObject> pTarget, const ENTITYDESC*) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;

public:
	_bool									Is_Target_InRange(_float fRange) const;

	void									Change_Phase(BROTHER_PHASE);

private:
	BROTHER_PHASE							m_ePhase			= BROTHER_PHASE::MAX;
	shared_ptr<class CBrother_Phase>		m_pPhase;

public:
	static shared_ptr<CBrother_Behavior>	Create(shared_ptr<class CGameObject> pOwner, shared_ptr<class CGameObject> pTarget, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>			Clone(any = g_aNull) override;
};

END
