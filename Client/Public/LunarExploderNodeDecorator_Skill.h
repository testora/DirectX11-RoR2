#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Client)

class CLunarExploderNodeDecorator_Skill final : public CDecorator
{
private:
	explicit CLunarExploderNodeDecorator_Skill(shared_ptr<CNode>, SKILLDESC*);
	virtual ~CLunarExploderNodeDecorator_Skill() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

private:
	SKILLDESC*												m_pSkillDesc	= nullptr;
	_bool													m_bAvailable	= false;

public:
	static shared_ptr<CLunarExploderNodeDecorator_Skill>	Create(shared_ptr<class CBlackBoard>, SKILLDESC*, shared_ptr<CNode>);
};

END
