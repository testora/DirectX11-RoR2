#pragma once

enum class KEY_STATE { DOWN, HOLD, UP, NONE, MAX };
		   
enum class SCENE;
		   
enum class RENDER_GROUP
{
	PRIORITY,
	NONBLEND,
	BLEND,
	UI,
	MAX
};

enum class COMPONENT
{
	RENDERER,
	TRANSFORM,
	SHADER,
	TEXTURE,
	MODEL,
	VIBUFFER,
	MESH,
	VIBUFFER_RECT,
	VIBUFFER_TERRAIN,
	MAX
};

enum class BEHAVIOR
{
	PHYSICS,
	GROUNDING,
	CONTROL,
	MAX
};

enum class PIPELINE		{ WORLD, VIEW, PROJ, MAX };
enum class TRANSFORM	{ RIGHT, UP, LOOK, POSITION, MAX };
enum class MODEL		{ NONANIM, ANIM, MAX };
enum class PICKER		{ LOCAL, WORLD, MAX };
