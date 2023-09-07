#pragma once
#include "Engine_Settings.h"

#pragma region Client

#define	ACTIVATE_MENU					FALSE
#define DEFAULT_FPS						60

#pragma endregion

#pragma region Device

#define BACK_BUFFER_COLOR				_float4(0.5f, 0.5f, 0.5f, 1.0f)

#pragma endregion

#pragma region INGAME - Camera

#define MAINCAM_SENSITIVITY_PITCH		75.0f
#define MAINCAM_SENSITIVITY_YAW			50.0f

#define MAINCAM_PITCH_MIN				-90.0f
#define MAINCAM_PITCH_MAX				+90.0f

#define MAINCAM_DEBUG_SPEED						25.f

#define MAINCAM_TOGGLE_MOUSE					VK_ESCAPE

#define MAINCAM_DEBUG_FORWARD					'W'
#define MAINCAM_DEBUG_LEFT						'A'
#define MAINCAM_DEBUG_BACKWARD					'S'
#define MAINCAM_DEBUG_RIGHT						'D'
#define MAINCAM_DEBUG_UP						VK_LSHIFT
#define MAINCAM_DEBUG_DOWN						VK_LCONTROL
#define MAINCAM_DEBUG_MOUSE_CONTROL				VK_MBUTTON

#pragma endregion

#pragma region INGAME - Player

#define PLAYER_CAMERA_OFFSET					_float4(0.f, 3.f, -6.f, 1.f)

#define PLAYER_SPEED_FORWARD					240.f;
#define PLAYER_SPEED_BACKWARD					160.f;
#define PLAYER_SPEED_LEFT						200.f;
#define PLAYER_SPEED_RIGHT						200.f;
#define PLAYER_JUMP_POWER						60.f;

#define PLAYER_SPEED_TERMINAL					_float3(200.f, 200.f, 200.f);
#define PLAYER_SPEED_RESIST						_float3(0.001f, 0.2f, 0.001f);

#define RAILGUNNER_MAIN_ATTACK_COOL				0.1f
#define	RAILGUNNER_RELOAD_COOL					1.f
#define	RAILGUNNER_RELOAD_SUCCESS_DELAY			0.14f
#define	RAILGUNNER_RELOAD_FAIL					0.1f

#define RAILGUNNER_SCOPE_FOV					XMConvertToRadians(10.f)
#define RAILGUNNER_SCOPE_ZOOM_WEIGHT			0.25f
#define RAILGUNNER_SCOPE_ZOOM_IN_DURATION		0.1f
#define RAILGUNNER_SCOPE_ZOOM_OUT_DURATION		0.2f

#pragma endregion
