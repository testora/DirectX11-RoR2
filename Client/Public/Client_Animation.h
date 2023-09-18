#pragma once

namespace ANIMATION
{
#pragma region PLAYABLE CHARACTER
	enum class RAILGUNNER
	{
	//	IDLE
		IDLE				= 15,
	//	AIR
		JUMP_START			= 29,
		AIR_LOOP_UP			= 10,
		AIR_LOOP_DOWN		= 3,
	//	LOOK
		AIM_PITCH			= 34,
		AIM_YAW				= 13,
	//	MOVEMENT
		RUN_FORWARD			= 24,
		RUN_BACKWARD		= 7,
		RUN_LEFT			= 23,
		RUN_RIGHT			= 35,
		SPRINT_FORWARD		= 27,
		SPRINT_LEFT			= 16,
		SPRINT_RIGHT		= 26,
	//	PISTOL
		PISTOL_CLICK		= 28,
		PISTOL_FIRE1		= 33,
		PISTOL_FIRE2		= 9,
		PISTOL_FIRE3		= 8,
		PISTOL_TO_SNIPER	= 12,
	//	SNIPER
		SNIPER_CLICK		= 17,
		SNIPER_FIRE			= 5,
		SNIPER_TO_PISTOL	= 6,
	//	SUPER
		SUPER_CHARGE		= 19,	// Before Ready: Slow Rotation
		SUPER_CHARGED		= 30,	// Ready : Rapid Rotation
		SUPER_FIRE			= 18,	// Fire
		SUPER_EXPIRE		= 4,	// Expired
		SUPER_WINDUP		= 0,	// After Expired : Rotation Loop Before Shot
	//	BACKPACK
		BACKPACK_IDLE		= 11,
		BACKPACK_CHARGE		= 37,	// Single Movement
		BACKPACK_CHARGED	= 1,	// Vibration Loop
		BACKPACK_OFFLINE	= 25,	// After Fire : Pop Pilar
		BACKPACK_REBOOT		= 21,	// After Penalty : Push in Pilar
	//	MINE
		MINE_THROW			= 14,	// Throw
	//	HIT
		HIT_LIGHTIMPACT		= 31,	// Hit
	//	SELECT
		SELECT_ENTRY		= 36,
		SELECT_IDLE			= 20,
		SELECT_LOOK1		= 22,
		SELECT_LOOK2		= 32,
	//	
		TEST				= 2		// IconicTestPose
	};
#pragma endregion
#pragma region MONSTER
	enum class GOLEM
	{
	//	IDLE
		IDLE				= 7,
		IDLE_POSE			= 8,
	//	AIR
		JUMP				= 9,
		AIR_LOOP_UP			= 0,
		AIR_LOOP_DOWN		= 1,
	//	LOOK
		LOOK_PITCH			= 11,
		LOOK_YAW			= 12,
	//	MOVEMENT
		WALK				= 22,
		RUN_FORWARD			= 14,
		RUN_BACKWARD		= 13,
		RUN_LEFT			= 15,
		RUN_RIGHT			= 16,
		RUN_TO_IDLE_LEFT	= 17,
		RUN_TO_IDLE_RIGHT	= 18,
	//	ATTACK
		ATTACK_SMACK		= 19,
		ATTACK_FIRELASER	= 3,
	//	HIT
		HIT_FLINCH1			= 4,
		HIT_FLINCH2			= 5,
		HIT_HURT1			= 6,
		HIT_LIGHTIMPACT		= 10,
	//	SPAWN
		DEATH				= 2,
		SPAWN				= 20,
	//	TEST
		TPOSE				= 21
	};
#pragma endregion
}
