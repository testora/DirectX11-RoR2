#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Client)

class CBrotherNode_Root final : public CNode
{
private:
	explicit CBrotherNode_Root() DEFAULT;
	virtual ~CBrotherNode_Root() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void							Activate() override;
	virtual STATUS							Invoke(_float fTimeDelta) override;
	virtual void							Terminate() override;

private:
	shared_ptr<CNode>						m_pPhaseSelector;

public:
	static shared_ptr<CBrotherNode_Root>	Create(shared_ptr<class CBlackBoard>);
};

END
