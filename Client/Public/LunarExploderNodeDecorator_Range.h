#pragma once
#include "Client_Define.h"
#include "Decorator.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CLunarExploderNodeDecorator_Range final : public CDecorator
{
private:
	explicit CLunarExploderNodeDecorator_Range(shared_ptr<CNode>, _float fDistance, _bool bInRange);
	explicit CLunarExploderNodeDecorator_Range(shared_ptr<CNode>, _float fDistanceMin, _float fDistanceMax);
	virtual ~CLunarExploderNodeDecorator_Range() DEFAULT;

public:
	virtual HRESULT											Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void											Activate() override;
	virtual STATUS											Invoke(_float fTimeDelta) override;
	virtual void											Terminate() override;

private:
	shared_ptr<CTransform>									m_pTransform;
	shared_ptr<CTransform>									m_pTargetTransform;

	_bool													m_bInRange = false;
	_float													m_fDistanceMin = 0.f;
	_float													m_fDistanceMax = 0.f;

public:
	static shared_ptr<CLunarExploderNodeDecorator_Range>	Create(shared_ptr<class CBlackBoard>, _float fDistance, _bool bInRange, shared_ptr<CNode>);
	static shared_ptr<CLunarExploderNodeDecorator_Range>	Create(shared_ptr<class CBlackBoard>, _float fDistanceMin, _float fDistanceMax, shared_ptr<CNode>);
};

END
