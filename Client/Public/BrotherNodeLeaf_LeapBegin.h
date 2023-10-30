#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CTransform;
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_LeapBegin final : public CNode
{
private:
	explicit CBrotherNodeLeaf_LeapBegin(BROTHER_PHASE*);
	virtual ~CBrotherNodeLeaf_LeapBegin() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CTransform>							m_pTransform;
	shared_ptr<CAnimator>							m_pAnimator;

	shared_ptr<CTransform>							m_pTargetTransform;

	shared_ptr<class CBrother>						m_pBrother;
	
	const BROTHER_PHASE								m_ePhase	= BROTHER_PHASE::MAX;
	BROTHER_PHASE*									m_pPhase	= nullptr;
	_bool											m_bExit		= false;

public:
	static shared_ptr<CBrotherNodeLeaf_LeapBegin>	Create(shared_ptr<class CBlackBoard>, BROTHER_PHASE* = nullptr);
};

END
