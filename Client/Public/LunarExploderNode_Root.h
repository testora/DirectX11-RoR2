#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Client)

class CLunarExploderNode_Root final : public CNode
{
private:
	explicit CLunarExploderNode_Root() DEFAULT;
	virtual ~CLunarExploderNode_Root() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void								Activate() override;
	virtual STATUS								Invoke(_float fTimeDelta) override;
	virtual void								Terminate() override;

private:
	shared_ptr<CNode>							m_pBehaviorSelector;

public:
	static shared_ptr<CLunarExploderNode_Root>	Create(shared_ptr<class CBlackBoard>);
};

END
