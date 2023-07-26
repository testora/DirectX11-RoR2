#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class C$safeitemname$ final : public CSingleton<C$safeitemname$>
{
    friend class CSingleton<C$safeitemname$>;
private:
	explicit C$safeitemname$();
	virtual ~C$safeitemname$() DEFAULT;
};

END
