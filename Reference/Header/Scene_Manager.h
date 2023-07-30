#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CScene_Manager final : public CSingleton<CScene_Manager>
{
	friend class CSingleton<CScene_Manager>;
};

END
