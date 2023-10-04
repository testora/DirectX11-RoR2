#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_LeapEnd final : public CNode
{
private:
	explicit CBrotherNodeLeaf_LeapEnd() DEFAULT;
	virtual ~CBrotherNodeLeaf_LeapEnd() DEFAULT;

public:
	virtual HRESULT						Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void						Activate() override;
	virtual STATUS						Invoke(_float fTimeDelta) override;
	virtual void						Terminate() override;

private:
	shared_ptr<CTransform>				m_pTransform;
	shared_ptr<CAnimator>				m_pAnimator;

public:
	static shared_ptr<CBrotherNodeLeaf_LeapEnd>	Create(shared_ptr<class CBlackBoard>);
};

END
