#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CLunarExploderNodeLeaf_Spawn final : public CNode
{
private:
	explicit CLunarExploderNodeLeaf_Spawn() DEFAULT;
	virtual ~CLunarExploderNodeLeaf_Spawn() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CAnimator>							m_pAnimator;

	_bool											m_bIsInitial	= true;

public:
	static shared_ptr<CLunarExploderNodeLeaf_Spawn>	Create(shared_ptr<class CBlackBoard>);
};

END
