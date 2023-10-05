#pragma once
#include "Client_Define.h"
#include "Sequence.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeSequence_Special final : public CSequence
{
private:
	explicit CBrotherNodeSequence_Special() DEFAULT;
	virtual ~CBrotherNodeSequence_Special() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	shared_ptr<CAnimator>							m_pAnimator;

	SKILLDESC*										m_pSkillDesc	= nullptr;

public:
	static shared_ptr<CBrotherNodeSequence_Special>	Create(shared_ptr<class CBlackBoard>);
};

END
