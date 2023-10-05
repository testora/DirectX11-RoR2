#pragma once

namespace ANIMATION
{
#pragma region PLAYABLE CHARACTER
	enum class RAILGUNNER
	{
	//	IDLE
		IDLE							= 15,
	//	AIR
		JUMP_START						= 29,
		AIR_LOOP_UP						= 10,
		AIR_LOOP_DOWN					= 3,
	//	LOOK
		AIM_PITCH						= 34,	// BONE
		AIM_YAW							= 13,	// BONE
	//	MOVEMENT
		RUN_FORWARD						= 24,
		RUN_BACKWARD					= 7,
		RUN_LEFT						= 23,
		RUN_RIGHT						= 35,
		SPRINT_FORWARD					= 27,
		SPRINT_LEFT						= 16,
		SPRINT_RIGHT					= 26,
	//	PISTOL
		PISTOL							= 28,	// POSE
		PISTOL_FIRE1					= 33,	// BONE
		PISTOL_FIRE2					= 9,	// BONE
		PISTOL_FIRE3					= 8,	// BONE
		PISTOL_TO_SNIPER				= 12,	// BONE
	//	SNIPER
		SNIPER							= 17,	// POSE
		SNIPER_FIRE						= 5,	// BONE
		SNIPER_TO_PISTOL				= 6,	// BONE
	//	SUPER
		SUPER_CHARGE					= 19,	// BONE		// Before Ready: Slow Rotation
		SUPER_CHARGED					= 30,	// BONE		// Ready : Rapid Rotation
		SUPER_FIRE						= 18,	// BONE		// Fire
		SUPER_EXPIRE					= 4,	// BONE		// Expired
		SUPER_WINDUP					= 0,	// BONE		// After Expired : Rotation Loop Before Shot
	//	BACKPACK
		BACKPACK_IDLE					= 11,	// BONE
		BACKPACK_CHARGE					= 37,	// BONE		// Single Movement
		BACKPACK_CHARGED				= 1,	// BONE		// Vibration Loop
		BACKPACK_OFFLINE				= 25,	// BONE		// After Fire : Pop Pilar
		BACKPACK_REBOOT					= 21,	// BONE		// After Penalty : Push in Pilar
	//	MINE
		MINE_THROW						= 14,	// BONE		// Throw
	//	HIT
		HIT_LIGHTIMPACT					= 31,				// Hit
	//	SELECT
		SELECT_ENTRY					= 36,
		SELECT_IDLE						= 20,
		SELECT_LOOK1					= 22,
		SELECT_LOOK2					= 32,
	//	TEST
		TEST							= 2,				// IconicTestPose

		MAX								= 38
	};
#pragma endregion
#pragma region MONSTER
	enum class BROTHER
	{
	//	IDLE
		IDLE_READY						= 17,
	//	SPELL
		SPELL_CHANNEL_ENTER				= 6,	// NO_WEAPON
		SPELL_CHANNEL_LOOP				= 13,	// NO_WEAPON
		SPELL_CHANNEL_TO_IDLE			= 14,	// NO_WEAPON
	//	AIR
		JUMP_FORWARD					= 44,
		AIR_LOOP_UP						= 21,
		AIR_LOOP_DOWN					= 32,
		AIR_LOOP_DOWN_FORWARD			= 19,
		AIR_LOOP_DOWN_BACKWARD			= 33,
	//	AIM
		AIM_YAW							= 31,	// BONE
		AIM_PITCH						= 29,	// BONE
	//	RUN
		RUN_FORWARD_READY				= 38,
		RUN_TO_IDLE						= 27,
	//	SPRINT
		SPRINT_FORWARD					= 23,
		SPRINT_SMASH					= 28,
	//	DASH
		DASH_FORWARD					= 35,
		DASH_BACKWARD					= 34,
		DASH_LEFT						= 36,
		DASH_RIGHT						= 37,
	//	SMASH
		SMASH_FORWARD					= 26,
	//	LUNARSHARD
		LUNARSHARD_FIRE_FORWARD			= 39,	// BONE
	//	ULT
		ULT_ENTER						= 43,
		ULT_CHANNEL						= 24,
		ULT_EXIT						= 48,
	//	FLINCH
		FLINCH1							= 47,	// BONE
		FLINCH2							= 40,	// BONE
		FLINCH3							= 41,	// BONE
	//	THRONE
		THRONE							= 25,
		THRONE_TO_IDLE					= 22,
	//	LEAP
		LEAP_BEGIN						= 45,
		LEAP_END						= 46,
	//	HIT
		LIGHTIMPACT						= 30,
	//	TEST
		TPOSE							= 20,	// TEST
#pragma region HURT
	//	IDLE
		HURT_IDLE_LOOP					= 0,	// NO_WEAPON
		HURT_IDLE_SINGLE				= 7,	// NO_WEAPON
	//	APPEAR
		HURT_FISTSLAM					= 42,	// NO_WEAPON
	//	WALK
		HURT_WALK_FORWARD				= 11,	// NO_WEAPON
	//	LUNARSHARD
		HURT_LUNARSHARD_ENTER_FORWARD	= 18,	// BONE
		HURT_LUNARSHARD_FIRE_FORWARD	= 3,	// BONE
		HURT_LUNARSHARD_FIRE_LEFT		= 4,	// BONE
		HURT_LUNARSHARD_FIRE_RIGHT		= 5,	// BONE
		HURT_LUNARSHARD_EXIT_FORWARD	= 15,	// BONE
		HURT_LUNARSHARD_EXIT_LEFT		= 1,	// BONE
		HURT_LUNARSHARD_EXIT_RIGHT		= 2,	// BONE
	//	STAGGER
		HURT_STAGGER_ENTER				= 8,	// NO_WEAPON
		HURT_STRGGER_EXIT				= 9,	// NO_WEAPON
		HURT_STAGGER_LOOP				= 10,	// NO_WEAPON
	//	DEATH
		HURT_TO_DEATH					= 12,
		DEATH_LOOP						= 16,
#pragma endregion
		MAX								= 49
	};

	enum class GOLEM
	{
	//	IDLE
		IDLE							= 7,
		IDLE_POSE						= 8,
	//	AIR
		JUMP							= 9,
		AIR_LOOP_UP						= 0,
		AIR_LOOP_DOWN					= 1,
	//	LOOK
		LOOK_PITCH						= 11,
		LOOK_YAW						= 12,
	//	MOVEMENT
		WALK							= 22,
		RUN_FORWARD						= 14,
		RUN_BACKWARD					= 13,
		RUN_LEFT						= 15,
		RUN_RIGHT						= 16,
		RUN_TO_IDLE_LEFT				= 17,
		RUN_TO_IDLE_RIGHT				= 18,
	//	ATTACK
		ATTACK_SMACK					= 19,
		ATTACK_FIRELASER				= 3,
	//	HIT
		HIT_FLINCH1						= 4,
		HIT_FLINCH2						= 5,
		HIT_HURT1						= 6,
		HIT_LIGHTIMPACT					= 10,
	//	SPAWN
		DEATH							= 2,
		SPAWN							= 20,
	//	TEST
		TPOSE							= 21,

		MAX								= 23
	};
#pragma endregion
}
