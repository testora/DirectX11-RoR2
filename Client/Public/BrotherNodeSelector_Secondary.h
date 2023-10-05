#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_Secondary final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Secondary() DEFAULT;
	virtual ~CBrotherNodeSelector_Secondary() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

public:
	static shared_ptr<CBrotherNodeSelector_Secondary>	Create(shared_ptr<class CBlackBoard>);
};

END
