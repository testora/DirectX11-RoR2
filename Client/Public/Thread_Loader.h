#pragma once
#include "Loader.h"
#include "EnginePCH.h"

UINT APIENTRY Thread_Loader_Main(void* _pArg)
{
	CLoader* pLoader = static_cast<CLoader*>(_pArg);

	if (FAILED(CoInitializeEx(nullptr, 0)))
	{
		assert(false);
	}

	if (FAILED(pLoader->Loading()))
	{
		assert(false);
	}

	return 0;
}
