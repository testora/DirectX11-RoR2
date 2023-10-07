#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_HurtIdle final : public CNode
{
private:
	explicit CBrotherNodeLeaf_HurtIdle() DEFAULT;
	virtual ~CBrotherNodeLeaf_HurtIdle() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CAnimator>							m_pAnimator;

public:
	static shared_ptr<CBrotherNodeLeaf_HurtIdle>	Create(shared_ptr<class CBlackBoard>);
};

END
