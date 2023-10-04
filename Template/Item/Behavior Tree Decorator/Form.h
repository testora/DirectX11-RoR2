#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Engine)
END

BEGIN(Client)

class C$safeitemname$ final : public CDecorator
{
private:
	explicit C$safeitemname$(shared_ptr<CNode>);
	virtual ~C$safeitemname$() DEFAULT;

public:
	virtual HRESULT						Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void						Activate() override;
	virtual STATUS						Invoke(_float fTimeDelta) override;
	virtual void						Terminate() override;

public:
	static shared_ptr<C$safeitemname$>	Create(shared_ptr<class CBlackBoard>, shared_ptr<CNode>);
};

END
