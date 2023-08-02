#pragma once
#include "Loader.h"
#include "EnginePCH.h"

UINT APIENTRY Thread_Loader_Main(void* _pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(_pArg);

	CoInitializeEx(nullptr, 0);

	if (FAILED(pLoader->Loading()))
	{
		assert(false);
	}

	return 0;
}
