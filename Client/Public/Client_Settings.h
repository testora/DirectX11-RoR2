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

#define MAINCAM_DEBUG_SPEED				25.f

#define MAINCAM_TOGGLE_MOUSE			VK_ESCAPE

#define MAINCAM_DEBUG_FORWARD			'W'
#define MAINCAM_DEBUG_LEFT				'A'
#define MAINCAM_DEBUG_BACKWARD			'S'
#define MAINCAM_DEBUG_RIGHT				'D'
#define MAINCAM_DEBUG_UP				VK_LSHIFT
#define MAINCAM_DEBUG_DOWN				VK_LCONTROL
#define MAINCAM_DEBUG_MOUSE_CONTROL		VK_MBUTTON

#pragma endregion

#pragma region INGAME - Player

#define PLAYER_CAMERA_OFFSET			_float4(0.f, 4.f, -10.f, 1.f)

#define PLAYER_SPEED_FORWARD			240.f;
#define PLAYER_SPEED_BACKWARD			160.f;
#define PLAYER_SPEED_LEFT				200.f;
#define PLAYER_SPEED_RIGHT				200.f;
#define PLAYER_JUMP_POWER				60.f;

#define PLAYER_SPEED_TERMINAL			_float3(200.f, 200.f, 200.f);
#define PLAYER_SPEED_RESIST				_float3(0.001f, 0.2f, 0.001f);

#pragma endregion
