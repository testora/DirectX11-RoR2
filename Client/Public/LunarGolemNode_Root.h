#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
END

BEGIN(Client)

class CLunarGolemNode_Root final : public CNode
{
private:
	explicit CLunarGolemNode_Root() DEFAULT;
	virtual ~CLunarGolemNode_Root() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void								Activate() override;
	virtual STATUS								Invoke(_float fTimeDelta) override;
	virtual void								Terminate() override;

private:
	HRESULT										Ready_Skills();

private:
	shared_ptr<CNode>							m_pBehaviorSelector;

	map<wstring, SKILLDESC>						m_mapSkills;

public:
	static shared_ptr<CLunarGolemNode_Root>		Create(shared_ptr<class CBlackBoard>);
};

END
