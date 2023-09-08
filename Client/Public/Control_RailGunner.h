#pragma once
#include "Client_Define.h"
#include "Control.h"

BEGIN(Client)

class CControl_RailGunner final : public CControl
{
private:
	enum class STATE	{ FORWARD, BACKWARD, LEFT, RIGHT, JUMP, MAX };

private:
	explicit CControl_RailGunner() DEFAULT;
	virtual ~CControl_RailGunner() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, shared_ptr<class CRailGunner_Crosshair>, const CHARACTERDESC*);
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;

protected:
	virtual void							Handle_MouseInput(_float fTimeDelta) override;
	virtual void							Handle_KeyInput(_float fTimeDelta) override;

private:
	_bool									m_bSnipeAvailable	= false;

	shared_ptr<class CRailGunner>			m_pRailGunner;
	shared_ptr<class CRailGunner_Crosshair>	m_pCrosshair;

public:
	static shared_ptr<CControl_RailGunner>	Create(shared_ptr<class CGameObject> pOwner, shared_ptr<class CRailGunner_Crosshair>, const CHARACTERDESC*);
	virtual shared_ptr<CBehavior>			Clone(any = g_aNull) override;
};

END
