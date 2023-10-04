#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Client)

class CBrotherNodeSelector_Phase0 final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase0() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase0() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>);

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	const BROTHER_PHASE*							m_pPhase	= nullptr;

public:
	static shared_ptr<CBrotherNodeSelector_Phase0>	Create(shared_ptr<class CBlackBoard>);
};

END
