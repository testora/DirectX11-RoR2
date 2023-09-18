#pragma once
#include "framework.h"

#include "Public_Define.h"
#include "Engine_Define.h"

namespace Client
{
	constexpr _uint	g_iWinCX	= 1920;
	constexpr _uint	g_iWinCY	= 1080;
}

using namespace Client;

extern HWND		g_hWnd;

#include "Client_Animation.h"
#include "Client_Enum.h"
#include "Client_Function.h"
#include "Client_Variable.h"
