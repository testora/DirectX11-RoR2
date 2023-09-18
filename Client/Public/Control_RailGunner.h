#pragma once
#include "Client_Define.h"
#include "Control.h"

BEGIN(Client)

class CControl_RailGunner final : public CControl
{
private:
	enum class STATE	{ FORWARD, BACKWARD, LEFT, RIGHT, MAX };

private:
	explicit CControl_RailGunner() DEFAULT;
	virtual ~CControl_RailGunner() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;

private:
	virtual void							Handle_MouseInput(_float fTimeDelta) override;
	virtual void							Handle_KeyInput(_float fTimeDelta) override;
	void									Handle_Bitset();

private:
	bitset<IDX(STATE::MAX)>					m_bitState;

	_bool									m_bSnipeAvailable	= false;

	shared_ptr<class CRailGunner>			m_pRailGunner;

public:
	static shared_ptr<CControl_RailGunner>	Create(shared_ptr<class CGameObject> pOwner, const ENTITYDESC*);
	virtual shared_ptr<CBehavior>			Clone(any = g_aNull) override;
};

END
