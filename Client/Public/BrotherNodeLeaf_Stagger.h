#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_Stagger final : public CNode
{
private:
	explicit CBrotherNodeLeaf_Stagger() DEFAULT;
	virtual ~CBrotherNodeLeaf_Stagger() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void								Activate() override;
	virtual STATUS								Invoke(_float fTimeDelta) override;
	virtual void								Terminate() override;

private:
	shared_ptr<CAnimator>						m_pAnimator;

public:
	static shared_ptr<CBrotherNodeLeaf_Stagger>	Create(shared_ptr<class CBlackBoard>);
};

END
