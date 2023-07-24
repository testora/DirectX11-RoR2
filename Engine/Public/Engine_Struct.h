#pragma once

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE	{ FULLSCREEN, WINDOW, MAX };
		HWND			hWnd;
		WINMODE			eWinMode;
		_uint			iWinCX, iWinCY;

	}GRAPHICDESC;
}