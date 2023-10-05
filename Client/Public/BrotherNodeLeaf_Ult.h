#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_Ult final : public CNode
{
private:
	explicit CBrotherNodeLeaf_Ult() DEFAULT;
	virtual ~CBrotherNodeLeaf_Ult() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void							Activate() override;
	virtual STATUS							Invoke(_float fTimeDelta) override;
	virtual void							Terminate() override;

private:
	shared_ptr<CAnimator>					m_pAnimator;

	DIRECTION								m_eDirection	= DIRECTION::MAX;

public:
	static shared_ptr<CBrotherNodeLeaf_Ult>	Create(shared_ptr<class CBlackBoard>);
};

END
