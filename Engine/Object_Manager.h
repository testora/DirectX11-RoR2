#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CObject_Manager final : public CSingleton<CObject_Manager>
{
    friend class CSingleton<CObject_Manager>;
};

END
