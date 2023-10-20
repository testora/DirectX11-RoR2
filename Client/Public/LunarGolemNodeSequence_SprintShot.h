#pragma once
#include "Client_Define.h"
#include "Sequence.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLunarGolemNodeSequence_SprintShot final : public CSequence
{
private:
	explicit CLunarGolemNodeSequence_SprintShot() DEFAULT;
	virtual ~CLunarGolemNodeSequence_SprintShot() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

public:
	static shared_ptr<CLunarGolemNodeSequence_SprintShot>	Create(shared_ptr<class CBlackBoard>);
};

END
