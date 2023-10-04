#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
END

BEGIN(Client)

class C$safeitemname$ final : public CNode
{
private:
	explicit C$safeitemname$() DEFAULT;
	virtual ~C$safeitemname$() DEFAULT;

public:
	virtual HRESULT						Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void						Activate() override;
	virtual STATUS						Invoke(_float fTimeDelta) override;
	virtual void						Terminate() override;

public:
	static shared_ptr<C$safeitemname$>	Create(shared_ptr<class CBlackBoard>);
};

END
