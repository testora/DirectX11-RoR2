#pragma once
#include "Client_Define.h"
#include "System.h"

BEGIN(Client)

class CGolem_Target final : public ISystem
{
private:
	explicit CGolem_Target() DEFAULT;
	explicit CGolem_Target(const CGolem_Target&) DEFAULT;
	virtual ~CGolem_Target() DEFAULT;

public:
	HRESULT								Initialize(shared_ptr<class CGolem>);
	virtual void						Tick(_float fTimeDelta) override;
	virtual void						Late_Tick(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	shared_ptr<class CGolem>			m_pGolem;

public:
	static shared_ptr<CGolem_Target>	Create(shared_ptr<class CGolem>);
};

END
