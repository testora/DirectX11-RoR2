#pragma once
#include "Client_Define.h"

BEGIN(Client)

class ISystem abstract : public std::enable_shared_from_this<ISystem>
{
protected:
	explicit ISystem() DEFAULT;
	virtual ~ISystem() DEFAULT;

public:
	virtual HRESULT		Initialize(any)					PURE;
	virtual void		Tick(_float fTimeDelta)			PURE;
	virtual void		Late_Tick(_float fTimeDelta)	PURE;
	virtual HRESULT		Render()						PURE;
};

END
