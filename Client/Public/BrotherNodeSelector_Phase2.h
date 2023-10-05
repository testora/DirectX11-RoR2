#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_Phase2 final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase2() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase2() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	BROTHER_PHASE*									m_pPhase	= nullptr;

public:
	static shared_ptr<CBrotherNodeSelector_Phase2>	Create(shared_ptr<class CBlackBoard>);
};

END
