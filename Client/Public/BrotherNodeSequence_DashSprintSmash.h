#pragma once
#include "Client_Define.h"
#include "Sequence.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSequence_DashSprintSmash final : public CSequence
{
private:
	explicit CBrotherNodeSequence_DashSprintSmash() DEFAULT;
	virtual ~CBrotherNodeSequence_DashSprintSmash() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

public:
	static shared_ptr<CBrotherNodeSequence_DashSprintSmash>	Create(shared_ptr<class CBlackBoard>);
};

END