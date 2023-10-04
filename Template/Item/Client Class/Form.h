#pragma once
#include "Client_Define.h"

BEGIN(Client)

class C$safeitemname$ final
{
private:
	explicit C$safeitemname$();
	explicit C$safeitemname$(const C$safeitemname$&);
	virtual ~C$safeitemname$() DEFAULT;

public:
	static shared_ptr<C$safeitemname$>	Create();
};

END
