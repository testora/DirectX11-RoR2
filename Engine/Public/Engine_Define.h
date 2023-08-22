#pragma once
#include "Public_Define.h"

constexpr _float	g_fEpsilon		= std::numeric_limits<float>::epsilon();
constexpr _float	g_fTolorance	= 1e-5f;
constexpr _float	g_fGravity		= 9.8f * 5.f;

constexpr _uint		g_iMaxLights	= 64;
constexpr _uint		g_iMaxBones		= 256;

const XMMATRIX		g_mUnit			= XMMatrixIdentity();
const any			g_aNull			= any();

#include "Engine_Settings.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"

using namespace Engine;

#include "Factory.h"
