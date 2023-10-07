#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_SpellChannel final : public CNode
{
private:
	explicit CBrotherNodeLeaf_SpellChannel() DEFAULT;
	virtual ~CBrotherNodeLeaf_SpellChannel() DEFAULT;

public:
	virtual HRESULT										Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void										Activate() override;
	virtual STATUS										Invoke(_float fTimeDelta) override;
	virtual void										Terminate() override;

private:
	shared_ptr<CTransform>								m_pTransform;
	shared_ptr<CAnimator>								m_pAnimator;

	_bool												m_bIsInital = true;

public:
	static shared_ptr<CBrotherNodeLeaf_SpellChannel>	Create(shared_ptr<class CBlackBoard>);
};

END
