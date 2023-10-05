#pragma once
#include "Client_Define.h"
#include "Parallel.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeParallel_PrimaryShard final : public CParallel
{
private:
	explicit CBrotherNodeParallel_PrimaryShard();
	virtual ~CBrotherNodeParallel_PrimaryShard() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

public:
	static shared_ptr<CBrotherNodeParallel_PrimaryShard>	Create(shared_ptr<class CBlackBoard>);
};

END
