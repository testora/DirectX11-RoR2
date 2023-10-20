#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLunarGolemNodeSelector_Main final : public CSelector
{
private:
	explicit CLunarGolemNodeSelector_Main() DEFAULT;
	virtual ~CLunarGolemNodeSelector_Main() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

public:
	static shared_ptr<CLunarGolemNodeSelector_Main>	Create(shared_ptr<class CBlackBoard>);
};

END
