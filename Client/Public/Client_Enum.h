#pragma once

enum class	SCENE
{
	STATIC,
	LOADING,
	MENU,
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	TOOL,
#endif
#endif
	TEST,
	MAX
};
