#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Client)

class CBrotherNodeDecorator_Skill final : public CDecorator
{
private:
	explicit CBrotherNodeDecorator_Skill(shared_ptr<CNode>, SKILLDESC*);
	virtual ~CBrotherNodeDecorator_Skill() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	SKILLDESC*										m_pSkillDesc	= nullptr;
	_bool											m_bAvailable	= false;

public:
	static shared_ptr<CBrotherNodeDecorator_Skill>	Create(shared_ptr<class CBlackBoard>, SKILLDESC*, shared_ptr<CNode>);
};

END
