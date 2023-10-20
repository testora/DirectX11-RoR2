#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Client)

class CLunarGolemNodeDecorator_Range final : public CDecorator
{
private:
	explicit CLunarGolemNodeDecorator_Range(shared_ptr<CNode>, _float fDistance, _bool bInRange);
	explicit CLunarGolemNodeDecorator_Range(shared_ptr<CNode>, _float fDistanceMin, _float fDistanceMax);
	virtual ~CLunarGolemNodeDecorator_Range() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

private:
	shared_ptr<CTransform>								m_pTransform;
	shared_ptr<CTransform>								m_pTargetTransform;

	_bool												m_bInRange		= false;
	_float												m_fDistanceMin	= 0.f;
	_float												m_fDistanceMax	= 0.f;

public:
	static shared_ptr<CLunarGolemNodeDecorator_Range>	Create(shared_ptr<class CBlackBoard>, _float fDistance, _bool bInRange, shared_ptr<CNode>);
	static shared_ptr<CLunarGolemNodeDecorator_Range>	Create(shared_ptr<class CBlackBoard>, _float fDistanceMin, _float fDistanceMax, shared_ptr<CNode>);
};

END
