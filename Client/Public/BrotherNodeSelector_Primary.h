#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_Primary final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Primary() DEFAULT;
	virtual ~CBrotherNodeSelector_Primary() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

public:
	static shared_ptr<CBrotherNodeSelector_Primary>	Create(shared_ptr<class CBlackBoard>);
};

END
