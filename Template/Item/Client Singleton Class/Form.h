#pragma once
#include "Client_Define.h"

BEGIN(Client)

class C$safeitemname$ final : public CSingleton<C$safeitemname$>
{
private:
	explicit C$safeitemname$();
	virtual ~C$safeitemname$() DEFAULT;

	friend CSingleton<C$safeitemname$>;
};

END
