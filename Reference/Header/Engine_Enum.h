#pragma once

enum class KEY_STATE { DOWN, HOLD, UP, NONE, MAX };
		   
enum class SCENE;
		   
enum class RENDER_GROUP
{
	CAMERA,
	PRIORITY,
	NONBLEND,
	NONLIGHT,
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
	COLLIDER,
	NAVIGATION,
	MODEL,
	VIBUFFER,
	MESH,
	VIBUFFER_RECT,
	VIBUFFER_TERRAIN,
	VIBUFFER_CELL,
	VIBUFFER_INSTANCE_POINT,
	VIBUFFER_INSTANCE_LINE,
	VIBUFFER_INSTANCE_RECT,
	MAX
};

enum class BEHAVIOR
{
	PHYSICS,
	GROUNDING,
	ANIMATOR,
	CONTROL,
	CUSTOM,
	MAX
};

enum class STATUS
{
	RUNNING,
	SUCCESS,
	FAILURE,
	MAX
};

enum class PIPELINE		{ WORLD, VIEW, PROJECTION, MAX };
enum class TRANSFORM	{ RIGHT, UP, LOOK, POSITION, MAX };
enum class MODEL		{ ANIM, NONANIM, MAX };
enum class COLLIDER		{ SPHERE, AABB, OBB, MAX };
enum class PICKER		{ LOCAL, WORLD, MAX };
