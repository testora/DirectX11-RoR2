#pragma once

namespace ANIMATION
{
	struct PLAYER
	{
		enum class IDLE
		{
			IDLE			= 16
		};
		enum class MOVE
		{
			RUN_FORWARD		= 22,
			RUN_BACKWARD	= 21,
			RUN_LEFT		= 23,
			RUN_RIGHT		= 24,
			SPRINT_FORWARD	= 27,
			SPRINT_LEFT		= 28,
			SPRINT_RIGHT	= 29
		};
		enum class JUMP
		{
			JUMP			= 17,
			LOOP_UP			= 3,
			LOOP_DOWN		= 2
		};
		enum class AIM
		{
			PITCH			= 0,
			YAW				= 1
		};
		enum class PISTOL
		{
			CLICK			= 20,
			FIRE1			= 11,
			FIRE2			= 12,
			FIRE3			= 13,
			TO_SNIPER		= 19
		};
		enum class SNIPER
		{
			CLICK			= 26,
			FIRE			= 14,
			TO_PISTOL		= 25
		};
		enum class SUPER
		{
			FIRE			= 15,
			CHARGE			= 10,
			CHARGED			= 9
		};
		enum class BACKPACK
		{
			IDLE			= 6,
			CHARGE			= 5,
			CHARGED			= 4,
			OFFLINE			= 7,
			REBOOT			= 8
		};
		enum class ETC
		{
			THROW_MINE		= 30,
			WINDUP_SUPER	= 31,
			LIGHTIMPACT		= 18
		};
	};
}
