#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CAnimator;
END

BEGIN(Client)

class CLunarGolemNodeLeaf_Death final : public CNode
{
private:
	explicit CLunarGolemNodeLeaf_Death() DEFAULT;
	virtual ~CLunarGolemNodeLeaf_Death() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CTransform>							m_pTransform;
	shared_ptr<CAnimator>							m_pAnimator;

	const ENTITYDESC*								m_pEntityDesc	= nullptr;

public:
	static shared_ptr<CLunarGolemNodeLeaf_Death>	Create(shared_ptr<class CBlackBoard>);
};

END
