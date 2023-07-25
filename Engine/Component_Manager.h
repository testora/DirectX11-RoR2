#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CComponent_Manager final : public CSingleton<CComponent_Manager>
{
    friend class CSingleton<CComponent_Manager>;
};

END
