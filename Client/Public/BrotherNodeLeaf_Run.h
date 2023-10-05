#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_Run final : public CNode
{
private:
	explicit CBrotherNodeLeaf_Run() DEFAULT;
	virtual ~CBrotherNodeLeaf_Run() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void							Activate() override;
	virtual STATUS							Invoke(_float fTimeDelta) override;
	virtual void							Terminate() override;

private:
	shared_ptr<CTransform>					m_pTransform;
	shared_ptr<CPhysics>					m_pPhysics;
	shared_ptr<CAnimator>					m_pAnimator;

	shared_ptr<CTransform>					m_pTargetTransform;

	const ENTITYDESC*						m_pEntityDesc	= nullptr;

public:
	static shared_ptr<CBrotherNodeLeaf_Run>	Create(shared_ptr<class CBlackBoard>);
};

END