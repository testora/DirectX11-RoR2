#pragma once

namespace ANIMATION
{
	struct PLAYER
	{
		enum class IDLE
		{
			IDLE			= 15
		};
		enum class MOVE
		{
			RUN_FORWARD		= 24,
			RUN_BACKWARD	= 7,
			RUN_LEFT		= 23,
			RUN_RIGHT		= 35,
			SPRINT_FORWARD	= 27,
			SPRINT_LEFT		= 16,
			SPRINT_RIGHT	= 26
		};
		enum class JUMP
		{
			JUMP			= 29,
			LOOP_UP			= 10,
			LOOP_DOWN		= 3
		};
		enum class AIM
		{
			PITCH			= 34,
			YAW				= 13
		};
		enum class PISTOL
		{
			CLICK			= 28,
			FIRE1			= 33,
			FIRE2			= 9,
			FIRE3			= 8,
			TO_SNIPER		= 12
		};
		enum class SNIPER
		{
			CLICK			= 17,
			FIRE			= 5,
			TO_PISTOL		= 6
		};
		enum class SUPER
		{
			CHARGE			= 19,	// Before Ready: Slow Rotation
			CHARGED			= 30,	// Ready : Rapid Rotation
			FIRE			= 18,	// Fire
			EXPIRE			= 4,	// Expired
			WINDUP			= 0		// After Expired : Rotation Loop Before Shot
		};
		enum class BACKPACK
		{
			IDLE			= 11,
			CHARGE			= 37,	// Single Movement
			CHARGED			= 1,	// Vibration Loop
			OFFLINE			= 25,	// After Fire : Pop Pilar
			REBOOT			= 21	// After Penalty : Push in Pilar
		};
		enum class MINE
		{
			THROW			= 14	// Throw
		};
		enum class HIT
		{
			LIGHTIMPACT		= 31	// Hit
		};
		enum class SELECT
		{
			ENTRY			= 36,
			IDLE			= 20,
			LOOK1			= 22,
			LOOK2			= 32,
		};
		enum class ETC
		{
			TEST			= 2		// IconicTestPose
		};
	};
}
