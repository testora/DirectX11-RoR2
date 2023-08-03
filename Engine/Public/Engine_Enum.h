#pragma once

enum class	SCENE;

enum class	RENDER_GROUP
{
	PRIORITY,
	NONBLEND,
	BLEND,
	UI,
	MAX
};

enum class	COMPONENT
{
	RENDERER,
	TRANSFORM,
	SHADER,
	TEXTURE,
	VIBUFFER_RECT,
	MAX
};

enum class	BEHAVIOR
{
	PHYSICS,
	MAX
};
