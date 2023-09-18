#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Client)

class CGolem_Hit final : public ISystem
{
private:
	explicit CGolem_Hit() DEFAULT;
	explicit CGolem_Hit(const CGolem_Hit&) DEFAULT;
	virtual ~CGolem_Hit() DEFAULT;

public:
	HRESULT								Initialize(shared_ptr<class CGolem>);
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	shared_ptr<class CGolem>			m_pGolem;

public:
	static shared_ptr<CGolem_Hit>		Create(shared_ptr<class CGolem>);
};

END
