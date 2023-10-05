#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_LunarPrimaryMelee final : public CSelector
{
private:
	explicit CBrotherNodeSelector_LunarPrimaryMelee() DEFAULT;
	virtual ~CBrotherNodeSelector_LunarPrimaryMelee() DEFAULT;

public:
	virtual HRESULT												Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void												Activate() override;
	virtual STATUS												Invoke(_float fTimeDelta) override;
	virtual void												Terminate() override;

public:
	static shared_ptr<CBrotherNodeSelector_LunarPrimaryMelee>	Create(shared_ptr<class CBlackBoard>);
};

END
