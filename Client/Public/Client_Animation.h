#pragma once

namespace ANIMATION
{
	enum class RAILGUNNER
	{
#pragma region IDLE
		IDLE				= 15,
#pragma endregion
#pragma region MOVEMENT
		RUN_FORWARD			= 24,
		RUN_BACKWARD		= 7,
		RUN_LEFT			= 23,
		RUN_RIGHT			= 35,
		SPRINT_FORWARD		= 27,
		SPRINT_LEFT			= 16,
		SPRINT_RIGHT		= 26,
#pragma endregion
#pragma region JUMP
		JUMP_START			= 29,
		JUMP_LOOP_UP		= 10,
		JUMP_LOOP_DOWN		= 3,
#pragma endregion
#pragma region AIM
		AIM_PITCH			= 34,
		AIM_YAW				= 13,
#pragma endregion
#pragma region PISTOL
		PISTOL_CLICK		= 28,
		PISTOL_FIRE1		= 33,
		PISTOL_FIRE2		= 9,
		PISTOL_FIRE3		= 8,
		PISTOL_TO_SNIPER	= 12,
#pragma endregion
#pragma region SNIPER
		SNIPER_CLICK		= 17,
		SNIPER_FIRE			= 5,
		SNIPER_TO_PISTOL	= 6,
#pragma endregion
#pragma region SUPER
		SUPER_CHARGE		= 19,	// Before Ready: Slow Rotation
		SUPER_CHARGED		= 30,	// Ready : Rapid Rotation
		SUPER_FIRE			= 18,	// Fire
		SUPER_EXPIRE		= 4,	// Expired
		SUPER_WINDUP		= 0,	// After Expired : Rotation Loop Before Shot
#pragma endregion
#pragma region BACKPACK
		BACKPACK_IDLE		= 11,
		BACKPACK_CHARGE		= 37,	// Single Movement
		BACKPACK_CHARGED	= 1,	// Vibration Loop
		BACKPACK_OFFLINE	= 25,	// After Fire : Pop Pilar
		BACKPACK_REBOOT		= 21,	// After Penalty : Push in Pilar
#pragma endregion
#pragma region MINE
		MINE_THROW			= 14,	// Throw
#pragma endregion
#pragma region HIT
		HIT_LIGHTIMPACT		= 31,	// Hit
#pragma endregion
#pragma region SELECT
		SELECT_ENTRY		= 36,
		SELECT_IDLE			= 20,
		SELECT_LOOK1		= 22,
		SELECT_LOOK2		= 32,
#pragma endregion
		TEST				= 2		// IconicTestPose
	};
}
