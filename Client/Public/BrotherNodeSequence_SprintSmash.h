#pragma once
#include "Client_Define.h"
#include "Sequence.h"

BEGIN(Client)

class CBrotherNodeSequence_SprintSmash final : public CSequence
{
private:
	explicit CBrotherNodeSequence_SprintSmash() DEFAULT;
	virtual ~CBrotherNodeSequence_SprintSmash() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

public:
	static shared_ptr<CBrotherNodeSequence_SprintSmash>	Create(shared_ptr<class CBlackBoard>);
};

END
