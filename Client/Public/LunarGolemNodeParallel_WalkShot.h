#pragma once
#include "Client_Define.h"
#include "Parallel.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLunarGolemNodeParallel_WalkShot final : public CParallel
{
private:
	explicit CLunarGolemNodeParallel_WalkShot();
	virtual ~CLunarGolemNodeParallel_WalkShot() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

public:
	static shared_ptr<CLunarGolemNodeParallel_WalkShot>	Create(shared_ptr<class CBlackBoard>);
};

END
