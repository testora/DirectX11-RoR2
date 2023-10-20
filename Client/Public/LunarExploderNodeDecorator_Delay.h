#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Client)

class CLunarExploderNodeDecorator_Delay final : public CDecorator
{
private:
	explicit CLunarExploderNodeDecorator_Delay(shared_ptr<CNode>, _float fDelay);
	virtual ~CLunarExploderNodeDecorator_Delay() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

private:
	const _float											m_fDelay = 0.f;
	_float													m_fTimeAcc = 0.f;
	_bool													m_bSuccess = false;

public:
	static shared_ptr<CLunarExploderNodeDecorator_Delay>	Create(shared_ptr<class CBlackBoard>, _float fDelay, shared_ptr<CNode>);
};

END
