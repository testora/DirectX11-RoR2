#pragma once
#pragma region Pre-Settings
#ifndef FALSE
#define FALSE					0
#endif
#ifndef TRUE
#define TRUE					1
#endif
#pragma endregion

#define	ACTIVATE_CONSOLE		FALSE
#define ACTIVATE_IMGUI			FALSE
#define ACTIVATE_TOOL			TRUE

#define TEMP_TRIPLANER			TRUE
#define WIP_FRUSTRUM_CULLING	FALSE

#pragma region Post-Settings
#ifndef _DEBUG
#undef ACTIVATE_IMGUI
#define ACTIVATE_IMGUI			FALSE
#endif
#if !ACTIVATE_IMGUI
#undef ACTIVATE_TOOL
#define ACTIVATE_TOOL			FALSE
#endif
#pragma endregion
