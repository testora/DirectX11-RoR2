#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CLight_Manager final : public CSingleton<CLight_Manager>
{
private:
	explicit CLight_Manager() DEFAULT;
	virtual ~CLight_Manager() DEFAULT;

	friend CSingleton<CLight_Manager>;
};

END
