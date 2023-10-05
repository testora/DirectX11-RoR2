#pragma once
#include "Client_Define.h"
#include "Sequence.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSequence_LunarDashSmash final : public CSequence
{
private:
	explicit CBrotherNodeSequence_LunarDashSmash() DEFAULT;
	virtual ~CBrotherNodeSequence_LunarDashSmash() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

public:
	static shared_ptr<CBrotherNodeSequence_LunarDashSmash>	Create(shared_ptr<class CBlackBoard>);
};

END
