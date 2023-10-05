 #pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CPhysics;
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_DashSide final : public CNode
{
private:
	explicit CBrotherNodeLeaf_DashSide() DEFAULT;
	virtual ~CBrotherNodeLeaf_DashSide() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CTransform>							m_pTransform;
	shared_ptr<CPhysics>							m_pPhysics;
	shared_ptr<CAnimator>							m_pAnimator;

	shared_ptr<CTransform>							m_pTargetTransform;

public:
	static shared_ptr<CBrotherNodeLeaf_DashSide>	Create(shared_ptr<class CBlackBoard>);
};

END
