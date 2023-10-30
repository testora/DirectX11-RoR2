#pragma once

#pragma region GameObject

#define PROTOTYPE_GAMEOBJECT_CAMERA_MAIN							TEXT("Prototype:GameObject:Camera:Main")

#define PROTOTYPE_GAMEOBJECT_SKYBOX_SKY0							TEXT("Prototype:GameObject:Skybox:Sky0")
#define PROTOTYPE_GAMEOBJECT_SKYBOX_SKY1							TEXT("Prototype:GameObject:Skybox:Sky1")

#define PROTOTYPE_GAMEOBJECT_BACKGROUND								TEXT("Prototype:GameObject:Background")
#define PROTOTYPE_GAMEOBJECT_TERRAIN								TEXT("Prototype:GameObject:Terrain")
#define PROTOTYPE_GAMEOBJECT_TELEPORTER								TEXT("Prototype:GameObject:Teleporter")
#define PROTOTYPE_GAMEOBJECT_GOLEMPLAINS							TEXT("Prototype:GameObject:GolemPlains")
#define PROTOTYPE_GAMEOBJECT_MOON									TEXT("Prototype:GameObject:Moon")
#define PROTOTYPE_GAMEOBJECT_RAILGUNNER								TEXT("Prototype:GameObject:RailGunner")
#define PROTOTYPE_GAMEOBJECT_RAILGUNNER_PISTOLBULLET				TEXT("Prototype:GameObject:RailGunner:PistolBullet")
#define PROTOTYPE_GAMEOBJECT_BROTHER								TEXT("Prototype:GameObject:Brother")
#define PROTOTYPE_GAMEOBJECT_BROTHER_LUNARSHARD						TEXT("Prototype:GameObject:Brother:LunarShard")
#define PROTOTYPE_GAMEOBJECT_BROTHER_HEALTHBAR						TEXT("Prototype:GameObject:Brother:HealthBar")
#define PROTOTYPE_GAMEOBJECT_LUNAREXPLODER							TEXT("Prototype:GameObject:LunarExploder")
#define PROTOTYPE_GAMEOBJECT_LUNARGOLEM								TEXT("Prototype:GameObject:LunarGolem")
#define PROTOTYPE_GAMEOBJECT_GOLEM									TEXT("Prototype:GameObject:Golem")
#define PROTOTYPE_GAMEOBJECT_EFFECT_DEMO_POINT						TEXT("Prototype:GameObject:Effect:Demo:Point")
#define PROTOTYPE_GAMEOBJECT_EFFECT_DEMO_RECT						TEXT("Prototype:GameObject:Effect:Demo:Rect")
#define PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_LINE						TEXT("Prototype:GameObject:Effect:Trail:Line")
#define PROTOTYPE_GAMEOBJECT_EFFECT_TRAIL_QUAD						TEXT("Prototype:GameObject:Effect:Trail:Quad")
#define PROTOTYPE_GAMEOBJECT_EFFECT_PARTICLE_MESH					TEXT("Prototype:GameObject:Effect:Particle:Mesh")

#pragma	endregion
#pragma region Component

#define PROTOTYPE_COMPONENT_RENDERER_MAIN							TEXT("Prototype:Component:Renderer:Main")

#define PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX						TEXT("Prototype:Component:Shader:VTXPOSTEX")
#define PROTOTYPE_COMPONENT_SHADER_VTXCUBEPOSTEX					TEXT("Prototype:Component:Shader:VTXCUBEPOSTEX")
#define PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX						TEXT("Prototype:Component:Shader:VTXPOSNORTEX")
#define PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX					TEXT("Prototype:Component:Shader:VTXPOSNORTANTEX")
#define PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE				TEXT("Prototype:Component:Shader:VTXPOSNORTANTEXBONE")
#define PROTOTYPE_COMPONENT_SHADER_VTXMESH							PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX
#define PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM						PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE
#define PROTOTYPE_COMPONENT_SHADER_INSTANCE_POINT					TEXT("Prototype:Component:Shader:Instance:Point")
#define PROTOTYPE_COMPONENT_SHADER_INSTANCE_LINE					TEXT("Prototype:Component:Shader:Instance:Line")
#define PROTOTYPE_COMPONENT_SHADER_INSTANCE_RECT					TEXT("Prototype:Component:Shader:Instance:Rect")
#define PROTOTYPE_COMPONENT_SHADER_INSTANCE_MESH					TEXT("Prototype:Component:Shader:Instance:Mesh")

#define PROTOTYPE_COMPONENT_TEXTURE_SKY								TEXT("Prototype:Component:Texture:Sky")
#define PROTOTYPE_COMPONENT_TEXTURE_BACKGROUND						TEXT("Prototype:Component:Texture:Background")
#define PROTOTYPE_COMPONENT_TEXTURE_TERRAIN							TEXT("Prototype:Component:Texture:Terrain")
#define PROTOTYPE_COMPONENT_TEXTURE_HEIGHTMAP						TEXT("Prototype:Component:Texture:HeightMap")
#define PROTOTYPE_COMPONENT_TEXTURE_BRUSH							TEXT("Prototype:Component:Texture:Brush")
#define PROTOTYPE_COMPONENT_TEXTURE_EFFECT_DEMO						TEXT("Prototype:Component:Texture:Effect:Demo")
#define PROTOTYPE_COMPONENT_TEXTURE_EFFECT_BROTHER_HAMMERTRAIL		TEXT("Prototype:Component:Texture:Effect:Brother:Hammer")

#define PROTOTYPE_COMPONENT_COLLIDER								TEXT("Prototype:Component:Collider")

#define PROTOTYPE_COMPONENT_VIBUFFER_RECT							TEXT("Prototype:Component:VIBuffer:Rect")
#define PROTOTYPE_COMPONENT_VIBUFFER_CUBE							TEXT("Prototype:Component:VIBuffer:Cube")
#define PROTOTYPE_COMPONENT_VIBUFFER_TERRAIN						TEXT("Prototype:Component:VIBuffer:Terrain")
#define PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_POINT					TEXT("Prototype:Component:VIBuffer:Instance:Point")
#define PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_LINE					TEXT("Prototype:Component:VIBuffer:Instance:Line")
#define PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_RECT					TEXT("Prototype:Component:VIBuffer:Instance:Rect")
#define PROTOTYPE_COMPONENT_VIBUFFER_INSTANCE_PILLAR				TEXT("Prototype:Component:VIBuffer:Instance:Pillar")

#define PROTOTYPE_COMPONENT_MODEL_RAILGUNNER						TEXT("Prototype:Component:Model:RailGunner")
#define PROTOTYPE_COMPONENT_MODEL_BROTHER							TEXT("Prototype:Component:Model:Brother")
#define PROTOTYPE_COMPONENT_MODEL_BROTHER_LUNARSHARD				TEXT("Prototype:Component:Model:LunarShard")
#define PROTOTYPE_COMPONENT_MODEL_LUNAREXPLODER						TEXT("Prototype:Component:Model:LunarExploder")
#define PROTOTYPE_COMPONENT_MODEL_LUNARGOLEM						TEXT("Prototype:Component:Model:LunarGolem")
#define PROTOTYPE_COMPONENT_MODEL_GOLEM								TEXT("Prototype:Component:Model:Golem")
#define PROTOTYPE_COMPONENT_MODEL_GOLEMPLAINS						TEXT("Prototype:Component:Model:GolemPlains")
#define PROTOTYPE_COMPONENT_MODEL_TELEPORTER						TEXT("Prototype:Component:Model:Teleporter")
#define PROTOTYPE_COMPONENT_MODEL_MOON								TEXT("Prototype:Component:Model:Moon")

#define PROTOTYPE_COMPONENT_MODEL_SPHERE							TEXT("Prototype:Component:Model:Sphere")

#ifdef _DEBUG
#define PROTOTYPE_COMPONENT_MODEL_CUBE								TEXT("Prototype:Component:Model:Cube")
#endif
#pragma endregion

#pragma region Layer

#define LAYER_BACKGROUND											TEXT("Layer:Background")
#define LAYER_INTERACTABLE											TEXT("Layer:Interactable")

#define LAYER_LIGHT													TEXT("Layer:Light")
#define LAYER_CAMERA												TEXT("Layer:Camera")
#define LAYER_BACKGROUND											TEXT("Layer:Background")
#define LAYER_TERRAIN												TEXT("Layer:Terrain")
#define LAYER_PLAYER												TEXT("Layer:Player")

#pragma endregion
#pragma region Pool

#define POOL_MONSTER_BROTHER										TEXT("Pool:Monster:Brother")
#define POOL_MONSTER_BROTHER_LUNARSHARD								TEXT("Pool:Monster:Brother:LunarShard")
#define POOL_MONSTER_LUNAREXPLODER									TEXT("Pool:Monster:LunarExploder")
#define POOL_MONSTER_LUNARGOLEM										TEXT("Pool:Monster:LunarGolem")
#define POOL_MONSTER_GOLEM											TEXT("Pool:Monster:Golem")
#define POOL_RAILGUNNER_PISTOLBULLET								TEXT("Pool:RailGunner:PistolBullet")

#define POOL_EFFECT_DEMO_POINT										TEXT("Pool:Effect:Demo:Point")
#define POOL_EFFECT_DEMO_RECT										TEXT("Pool:Effect:Demo:Rect")
#define POOL_EFFECT_TRAIL_LINE										TEXT("Pool:Effect:Trail:Line")
#define POOL_EFFECT_TRAIL_QUAD										TEXT("Pool:Effect:Trail:Quad")
#define POOL_EFFECT_PARTICLE_MESH_PILLAR							TEXT("Pool:Effect:Particle:Mesh:Pillar")

#pragma endregion
#pragma region Grid Layer

#define GRID_TERRAIN												TEXT("Grid:Terrain") 

#pragma endregion

#define SHADER_TEXTARGET_SCOPE										"g_texScopeTarget"
