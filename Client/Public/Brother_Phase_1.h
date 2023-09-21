#pragma once
#include "Client_Define.h"
#include "Brother_Phase.h"

BEGIN(Client)

class CBrother_Phase_1 final : public CBrother_Phase
{
private:
	explicit CBrother_Phase_1() DEFAULT;
	virtual ~CBrother_Phase_1() DEFAULT;

public:
	virtual HRESULT							Initialize(shared_ptr<CBrother_Phase> pPrevious) override;
	virtual void							Tick(_float fTimeDelta) override;
	virtual void							Late_Tick(_float fTimeDelta) override;

private:
	virtual void							Handle_State(_float fTimeDelta) override;

public:
	static shared_ptr<CBrother_Phase_1>		Create(shared_ptr<CBrother_Phase> pPrevious);
};

END
