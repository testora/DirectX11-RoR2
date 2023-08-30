#pragma once

enum class	SCENE
{
	STATIC,
	LOADING,
	MENU,
#if ACTIVATE_TOOL
	TOOL,
#endif
	TEST,
	MAX
};
