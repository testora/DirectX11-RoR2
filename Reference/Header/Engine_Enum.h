#pragma once

enum class	SCENE
{
	STATIC,
	LOADING,
	LOGO,
	STAGE1,
	MAX
};

enum class	COMPONENT
{
	TRANSFORM = 0b1,
	RENDERER = 0b10,
	SHADER = 0b100,
	MAX
};

enum class	BEHAVIOR
{
	PHYSICS,
	MAX
};

enum class	TRANSFORM
{
	RIGHT,
	UP,
	LOOK,
	POSITION,
	MAX
};
