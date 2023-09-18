#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Client)

class CGolem_Untarget final : public ISystem
{
private:
	explicit CGolem_Untarget() DEFAULT;
	explicit CGolem_Untarget(const CGolem_Untarget&) DEFAULT;
	virtual ~CGolem_Untarget() DEFAULT;

public:
	HRESULT								Initialize(shared_ptr<class CGolem>);
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	_float								m_fTimeAcc	= 0.f;

	shared_ptr<class CGolem>			m_pGolem;

public:
	static shared_ptr<CGolem_Untarget>	Create(shared_ptr<class CGolem>);
};

END
