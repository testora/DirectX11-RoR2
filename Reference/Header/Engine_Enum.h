#pragma once

enum class	SCENE
{
	STATIC,
	LOADING,
	LOGO,
	STAGE1,
	MAX
};

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
	VIBUFFER,
	MAX
};

enum class	BEHAVIOR
{
	PHYSICS,
	MAX
};
