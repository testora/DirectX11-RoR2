#pragma once

#pragma region Window

#define	ACTIVATE_MENU			FALSE

#pragma endregion

#pragma region Device

#define BACK_BUFFER_COLOR		_float4(0.5f, 0.5f, 0.5f, 1.0f)

#pragma endregion

#pragma region Timer

#define DEFAULT_FPS				60

#pragma endregion

#pragma region INGAME - Camera

#define MAINCAM_SENSITIVITY		20.0f

#define MAINCAM_PITCH_MIN		-90.0f
#define MAINCAM_PITCH_MAX		+90.0f

#define MAINCAM_DEBUG_SPEED		5.f

#define MAINCAM_TOGGLE_MOUSE	VK_OEM_3

#define MAINCAM_DEBUG_FORWARD	'W'
#define MAINCAM_DEBUG_LEFT		'A'
#define MAINCAM_DEBUG_BACKWARD	'S'
#define MAINCAM_DEBUG_RIGHT		'D'
#define MAINCAM_DEBUG_UP		VK_LSHIFT
#define MAINCAM_DEBUG_DOWN		VK_LCONTROL

#pragma endregion
