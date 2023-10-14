#pragma once
#include "Engine_Settings.h"

#pragma region Client

#define	ACTIVATE_MENU							FALSE
#define DEFAULT_FPS								60

#pragma endregion

#pragma region Device

#define BACK_BUFFER_COLOR						_float4(0.5f, 0.5f, 0.5f, 1.0f)

#pragma endregion

#pragma region INGAME - Camera

#define MAINCAM_SENSITIVITY_YAW					1.f
#define MAINCAM_SENSITIVITY_PITCH				1.f

#define MAINCAM_PITCH_MIN						-90.0f
#define MAINCAM_PITCH_MAX						+90.0f
#define MAINCAM_YAW_MIN							-90.0f
#define MAINCAM_YAW_MAX							+90.0f

#define MAINCAM_DEBUG_SPEED						25.f

#define MAINCAM_TOGGLE_MOUSE					VK_ESCAPE

#define MAINCAM_DEBUG_FORWARD					'W'
#define MAINCAM_DEBUG_LEFT						'A'
#define MAINCAM_DEBUG_BACKWARD					'S'
#define MAINCAM_DEBUG_RIGHT						'D'
#define MAINCAM_DEBUG_UP						VK_LSHIFT
#define MAINCAM_DEBUG_DOWN						VK_LCONTROL
#define MAINCAM_DEBUG_MOUSE_CONTROL				VK_MBUTTON

#define MAINCAM_PISTOL_REBOUND_DURATION			0.1f

#define MAINCAM_SNIPER_REBOUND_DURATION			0.5f
#define MAINCAM_SNIPER_REBOUND_RATIO			0.2f
#define MAINCAM_SNIPER_RECOVER_RATIO			1.f - MAINCAM_SNIPER_REBOUND_RATIO

#pragma endregion

#pragma region INGAME - Control

#define CONTROL_FORWARD							'W'
#define CONTROL_LEFT							'A'
#define CONTROL_BACKWARD						'S'
#define CONTROL_RIGHT							'D'
#define CONTROL_JUMP							VK_SPACE
#define CONTROL_SPRINT							VK_CONTROL
#define CONTROL_ATTACK1							VK_LBUTTON
#define CONTROL_ATTACK2							VK_RBUTTON
#define CONTROL_UTILITY							VK_SHIFT
#define CONTROL_SPECIAL							'R'

#pragma endregion 

#pragma region INGAME - Player

#define PLAYER_CAMERA_OFFSET					_float4(0.f, 3.f, -6.f, 1.f)

#define PLAYER_SPEED_FORWARD					150.f
#define PLAYER_SPEED_BACKWARD					90.f
#define PLAYER_SPEED_LEFT						90.f
#define PLAYER_SPEED_RIGHT						90.f
#define PLAYER_SPRINT_POWER						1.5f
#define PLAYER_JUMP_POWER						60.f

#define PLAYER_SPEED_TERMINAL					_float3(200.f, 200.f, 200.f)
#define PLAYER_SPEED_RESIST						_float3(0.001f, 0.2f, 0.001f)

#define RAILGUNNER_COOLTIME_PISTOL				0.2f
#define	RAILGUNNER_RELOAD_COOL					1.f
#define	RAILGUNNER_RELOAD_SUCCESS_DELAY			0.14f
#define	RAILGUNNER_RELOAD_FAIL					0.1f

#define RAILGUNNER_SCOPE_FOV					XMConvertToRadians(10.f)
#define RAILGUNNER_SCOPE_ZOOM_WEIGHT			0.25f
#define RAILGUNNER_SCOPE_ZOOM_IN_DURATION		0.1f
#define RAILGUNNER_SCOPE_ZOOM_OUT_DURATION		0.2f

#define RAILGUNNER_SCOPE_SENSITIVITY_YAW		0.025f
#define RAILGUNNER_SCOPE_SENSITIVITY_PITCH		0.025f

#pragma region RailGunner - PistolBullet

#define PISTOLBULLET_SPEED_FORWARD				200.f

#define PISTOLBULLET_SPEED_TERMINAL				_float3(1000.f, 1000.f, 1000.f)
#define PISTOLBULLET_SPEED_RESIST				_float3(0.9f, 0.9f, 0.9f)

#define PLAYER_SPEED_TERMINAL					_float3(200.f, 200.f, 200.f)
#define PLAYER_SPEED_RESIST						_float3(0.001f, 0.2f, 0.001f)


#define PISTOLBULLET_LIFESPAN					2.f
#define PISTOLBULLET_TARGET_RANGE				20.f

#pragma endregion
#pragma region RailGunner - Sniper

#define WEAKPOINT_SCALE_FACTOR					_float3(5000.f, 5000.f, 5000.f)

#pragma endregion
#pragma endregion

#pragma region INGAME - Brother

#define BROTHER_SPEED_FORWARD					120.f
#define BROTHER_SPEED_BACKWARD					60.f
#define BROTHER_SPEED_LEFT						75.f
#define BROTHER_SPEED_RIGHT						75.f
#define BROTHER_SPRINT_POWER					1.5f
#define BROTHER_HURT_FORCE_POWER				0.2f
#define BROTHER_HURT_POWER						0.75f
#define BROTHER_HURT_INTERPOLATE				0.25f
#define BROTHER_JUMP_POWER						60.f

#define BROTHER_SPEED_TERMINAL					_float3(200.f, 200.f, 200.f)
#define BROTHER_SPEED_RESIST					_float3(0.02f, 0.2f, 0.02f)

#define BROTHER_MOVEMENT_TIME_MIN				1.5f
#define BROTHER_MOVEMENT_TIME_MAX				3.f
#define BROTHER_MOTION_GAP_MIN					0.5f
#define BROTHER_MOTION_GAP_MAX					1.f

#define	BROTHER_CHASE_LIMIT_DISTANCE			10.f
#define	BROTHER_SPRINT_SMASH_DISTANCE			50.f
#define	BROTHER_SPRINT_SMASH_FORCE				250.f

#define BROTHER_DASH_FORCE						100.f

#define BROTHER_EFFECT_DELAY_PILLAR				1.63f
#define BROTHER_EFFECT_OFFSET_PILLAR			_float3(0.f, 0.f, 8.f)

#pragma endregion
#pragma region INGAME - Golem

#define GOLEM_SPEED_FORWARD						90.f
#define GOLEM_SPEED_BACKWARD					60.f
#define GOLEM_SPEED_LEFT						75.f
#define GOLEM_SPEED_RIGHT						75.f
#define GOLEM_SPRINT_POWER						2.f
#define GOLEM_JUMP_POWER						60.f

#define GOLEM_SPEED_TERMINAL					_float3(200.f, 200.f, 200.f)
#define GOLEM_SPEED_RESIST						_float3(0.000001f, 0.2f, 0.000001f)

#define GOLEM_MOVEMENT_TIME_MIN					1.5f
#define GOLEM_MOVEMENT_TIME_MAX					3.f
#define GOLEM_MOTION_GAP_MIN					0.5f
#define GOLEM_MOTION_GAP_MAX					1.f

#pragma endregion

#pragma region MOON

#define	ARENA_POINT								_float3(-1900.f, 1060.f, 710.f)
#define	ARENA_CENTER							_float3(-2050.f, 993.126465f, 590.f)

#pragma endregion
