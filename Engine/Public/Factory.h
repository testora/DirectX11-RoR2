#pragma once
#include "Public_Define.h"
#include "Engine_Macro.h"

BEGIN(Engine)

template <typename T>
class ENGINE_DLL CFactory final
{
public:
	template <typename... Args>
	static shared_ptr<T> Create(Args&&... args)
	{
		return T::Create(args...);
	}
};

END
