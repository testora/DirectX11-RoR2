#pragma once
#include "Client_Define.h"
#include "Brother_Phase.h"

BEGIN(Client)

class CBrother_Phase_0 final : public CBrother_Phase
{
private:
	enum class STATE
	{
		APPEAR,
		READY,
		CHASE,
		CHASE_TO_SMASH,
		CHASE_SMASH,
		DASH,
		SMASH,
		MAX
	};

private:
	explicit CBrother_Phase_0() DEFAULT;
	virtual ~CBrother_Phase_0() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, shared_ptr<class CBrother_Behavior>, const ENTITYDESC*) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;

private:
	virtual void							Handle_State(_float fTimeDelta) override;

private:
	STATE									m_eState	= STATE::MAX;

public:
	static shared_ptr<CBrother_Phase_0>		Create(shared_ptr<class CGameObject> pOwner, shared_ptr<class CBrother_Behavior>, const ENTITYDESC*);
};

END
