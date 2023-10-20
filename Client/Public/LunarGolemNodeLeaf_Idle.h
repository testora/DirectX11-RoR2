#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CLunarGolemNodeLeaf_Idle final : public CNode
{
private:
	explicit CLunarGolemNodeLeaf_Idle() DEFAULT;
	virtual ~CLunarGolemNodeLeaf_Idle() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void								Activate() override;
	virtual STATUS								Invoke(_float fTimeDelta) override;
	virtual void								Terminate() override;

private:
	shared_ptr<CAnimator>						m_pAnimator;

public:
	static shared_ptr<CLunarGolemNodeLeaf_Idle>	Create(shared_ptr<class CBlackBoard>);
};

END
