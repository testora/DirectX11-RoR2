#pragma once
#include "Client_Define.h"
#include "Sequence.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSequence_DashSmash final : public CSequence
{
private:
	explicit CBrotherNodeSequence_DashSmash() DEFAULT;
	virtual ~CBrotherNodeSequence_DashSmash() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

public:
	static shared_ptr<CBrotherNodeSequence_DashSmash>	Create(shared_ptr<class CBlackBoard>);
};

END
