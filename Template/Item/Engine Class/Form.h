#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL C$safeitemname$ abstract
{
private:
	explicit C$safeitemname$();
	explicit C$safeitemname$(const C$safeitemname$&);
	virtual ~C$safeitemname$() DEFAULT;

public:
	static shared_ptr<C$safeitemname$>	Create();
};

END
