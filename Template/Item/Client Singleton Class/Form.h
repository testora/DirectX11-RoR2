#pragma once
#include "Client_Define.h"

BEGIN(Client)

class C$safeitemname$ final : public CSingleton<C$safeitemname$>
{
    friend class CSingleton<C$safeitemname$>;
private:
	explicit C$safeitemname$();
	virtual ~C$safeitemname$() DEFAULT;
};

END
