#pragma once
#include "Public_Define.h"

constexpr _float	g_fEpsilon						= std::numeric_limits<float>::epsilon();
constexpr _float	g_fTolorance					= 1e-5f;

constexpr _uint		g_iMaxTextures					= 8;
constexpr _uint		g_iMaxLights					= 64;
constexpr _uint		g_iMaxBones						= 256;

constexpr _float	g_fGravity						= 9.8f * 6.f;
constexpr _float	g_fDefaultInterpolationDuration	= 0.1f;
constexpr XMFLOAT3	g_vGridSize						= XMFLOAT3(100.f, 100.f, 100.f);

const XMMATRIX		g_mUnit							= XMMatrixIdentity();
const any			g_aNull							= any();

#ifndef AI_ASSIMP_HPP_INC
enum aiTextureType {
	aiTextureType_NONE					= 0,
	aiTextureType_DIFFUSE				= 1,
	aiTextureType_SPECULAR				= 2,
	aiTextureType_AMBIENT				= 3,
	aiTextureType_EMISSIVE				= 4,
	aiTextureType_HEIGHT				= 5,
	aiTextureType_NORMALS				= 6,
	aiTextureType_SHININESS				= 7,
	aiTextureType_OPACITY				= 8,
	aiTextureType_DISPLACEMENT			= 9,
	aiTextureType_LIGHTMAP				= 10,
	aiTextureType_REFLECTION			= 11,
	aiTextureType_BASE_COLOR			= 12,
	aiTextureType_NORMAL_CAMERA			= 13,
	aiTextureType_EMISSION_COLOR		= 14,
	aiTextureType_METALNESS				= 15,
	aiTextureType_DIFFUSE_ROUGHNESS		= 16,
	aiTextureType_AMBIENT_OCCLUSION		= 17,
	aiTextureType_SHEEN					= 19,
	aiTextureType_CLEARCOAT				= 20,
	aiTextureType_TRANSMISSION			= 21,
	aiTextureType_UNKNOWN				= 18,
#ifndef SWIG
	_aiTextureType_Force32Bit			= INT_MAX
#endif
};

#define AI_TEXTURE_TYPE_MAX				aiTextureType_TRANSMISSION
#endif

#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Shader.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"

using namespace Engine;

#include "Factory.h"
