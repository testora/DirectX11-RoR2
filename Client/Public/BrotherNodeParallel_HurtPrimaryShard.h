#pragma once
#include "Client_Define.h"
#include "Parallel.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeParallel_HurtPrimaryShard final : public CParallel
{
private:
	explicit CBrotherNodeParallel_HurtPrimaryShard();
	virtual ~CBrotherNodeParallel_HurtPrimaryShard() DEFAULT;

public:
	virtual HRESULT												Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void												Activate() override;
	virtual STATUS												Invoke(_float fTimeDelta) override;
	virtual void												Terminate() override;

public:
	static shared_ptr<CBrotherNodeParallel_HurtPrimaryShard>	Create(shared_ptr<class CBlackBoard>);
};

END
