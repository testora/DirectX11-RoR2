#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_PrimaryMelee final : public CSelector
{
private:
	explicit CBrotherNodeSelector_PrimaryMelee() DEFAULT;
	virtual ~CBrotherNodeSelector_PrimaryMelee() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

public:
	static shared_ptr<CBrotherNodeSelector_PrimaryMelee>	Create(shared_ptr<class CBlackBoard>);
};

END
