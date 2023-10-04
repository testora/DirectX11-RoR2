#pragma once
#include "Client_Define.h"
#include "Sequence.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSequence_Enter final : public CSequence
{
private:
	explicit CBrotherNodeSequence_Enter() DEFAULT;
	virtual ~CBrotherNodeSequence_Enter() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	_bool											m_bIsArrived	= false;
	_float											m_fTimeAcc		= 0.f;

public:
	static shared_ptr<CBrotherNodeSequence_Enter>	Create(shared_ptr<class CBlackBoard>);
};

END
