#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Client)

class CBrotherNodeSelector_Phase final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>);

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	BROTHER_PHASE									m_ePhase	= BROTHER_PHASE::PHASE1;

public:
	static shared_ptr<CBrotherNodeSelector_Phase>	Create(shared_ptr<class CBlackBoard>);
};

END
