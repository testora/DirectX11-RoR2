#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLunarExploderNodeSelector_Main final : public CSelector
{
private:
	explicit CLunarExploderNodeSelector_Main() DEFAULT;
	virtual ~CLunarExploderNodeSelector_Main() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

public:
	static shared_ptr<CLunarExploderNodeSelector_Main>	Create(shared_ptr<class CBlackBoard>);
};

END
