#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Client)

class CLunarGolemNodeDecorator_Repeat final : public CDecorator
{
private:
	explicit CLunarGolemNodeDecorator_Repeat(shared_ptr<CNode>, _float fDuration);
	explicit CLunarGolemNodeDecorator_Repeat(shared_ptr<CNode>, function<_bool()> fpEscape);
	virtual ~CLunarGolemNodeDecorator_Repeat() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

private:
	_float												m_fDuration	= 0.f;
	function<_bool()>									m_fpEscape	= nullptr;

public:
	static shared_ptr<CLunarGolemNodeDecorator_Repeat>	Create(shared_ptr<class CBlackBoard>, _float fDuration, shared_ptr<CNode>);
	static shared_ptr<CLunarGolemNodeDecorator_Repeat>	Create(shared_ptr<class CBlackBoard>, function<_bool()> fpEscape, shared_ptr<CNode>);
};

END