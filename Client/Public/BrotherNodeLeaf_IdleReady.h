#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_IdleReady final : public CNode
{
private:
	explicit CBrotherNodeLeaf_IdleReady() DEFAULT;
	virtual ~CBrotherNodeLeaf_IdleReady() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CAnimator>							m_pAnimator;

public:
	static shared_ptr<CBrotherNodeLeaf_IdleReady>	Create(shared_ptr<class CBlackBoard>);
};

END
