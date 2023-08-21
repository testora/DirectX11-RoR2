#pragma once

#pragma region GameObject

#define PROTOTYPE_GAMEOBJECT_CAMERA_MAIN					TEXT("Prototype:GameObject:Camera:Main")

#define PROTOTYPE_GAMEOBJECT_BACKGROUND						TEXT("Prototype:GameObject:Background")
#define PROTOTYPE_GAMEOBJECT_TERRAIN						TEXT("Prototype:GameObject:Terrain")
#define PROTOTYPE_GAMEOBJECT_RAILGUNNER						TEXT("Prototype:GameObject:RailGunner")

#pragma	endregion
#pragma region Component

#define PROTOTYPE_COMPONENT_RENDERER_MAIN					TEXT("Prototype:Component:Renderer:Main")

#define PROTOTYPE_COMPONENT_SHADER_VTXPOSTEX				TEXT("Prototype:Component:Shader:VTXPOSTEX")
#define PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTEX				TEXT("Prototype:Component:Shader:VTXPOSNORTEX")
#define PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX			TEXT("Prototype:Component:Shader:VTXPOSNORTANTEX")
#define PROTOTYPE_COMPONENT_SHADER_VTXMESH					PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEX
#define PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE		TEXT("Prototype:Component:Shader:VTXPOSNORTANTEXBONE")
#define PROTOTYPE_COMPONENT_SHADER_VTXMESHANIM				PROTOTYPE_COMPONENT_SHADER_VTXPOSNORTANTEXBONE

#define PROTOTYPE_COMPONENT_TEXTURE_BACKGROUND				TEXT("Prototype:Component:Texture:Background")
#define PROTOTYPE_COMPONENT_TEXTURE_TERRAIN					TEXT("Prototype:Component:Texture:Terrain")
#define PROTOTYPE_COMPONENT_TEXTURE_HEIGHTMAP				TEXT("Prototype:Component:Texture:HeightMap")
#define PROTOTYPE_COMPONENT_TEXTURE_BRUSH					TEXT("Prototype:Component:Texture:Brush")

#define PROTOTYPE_COMPONENT_VIBUFFER_RECT					TEXT("Prototype:Component:VIBuffer:Rect")
#define PROTOTYPE_COMPONENT_VIBUFFER_TERRAIN				TEXT("Prototype:Component:VIBuffer:Terrain")

#define PROTOTYPE_COMPONENT_MODEL_RAILGUNNER				TEXT("Prototype:Component:Model:RailGunner")

#pragma endregion
#pragma region Layer

#define SCENE_MENU_LAYER_BACKGROUND							TEXT("Scene:Menu:Layers:Background")

#define SCENE_TEST_LAYER_LIGHT								TEXT("Scene:Test:Layers:Light")
#define SCENE_TEST_LAYER_CAMERA								TEXT("Scene:Test:Layers:Camera")
#define SCENE_TEST_LAYER_TERRAIN							TEXT("Scene:Test:Layers:Terrain")
#define SCENE_TEST_LAYER_PLAYER								TEXT("Scene:Test:Layers:Player")

#pragma endregion