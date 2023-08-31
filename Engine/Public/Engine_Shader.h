#pragma once

#define SHADER_FLAG								"g_iShaderFlag"
#define SHADER_FLAG_TEXDIFFUSE					0b0000'0000'0000'0001
#define SHADER_FLAG_TEXNORMAL					0b0000'0000'0000'0010
#define SHADER_FLAG_TRIPLANER_POSITIVE_X		0b0000'0000'0000'0100
#define SHADER_FLAG_TRIPLANER_POSITIVE_Y		0b0000'0000'0000'1000
#define SHADER_FLAG_TRIPLANER_POSITIVE_Z		0b0000'0000'0001'0000
#define SHADER_FLAG_TRIPLANER_NEGATIVE_X		0b0000'0000'0010'0000
#define SHADER_FLAG_TRIPLANER_NEGATIVE_Y		0b0000'0000'0100'0000
#define SHADER_FLAG_TRIPLANER_NEGATIVE_Z		0b0000'0000'1000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_X			0b0000'0001'0000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_Y			0b0000'0010'0000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_Z			0b0000'0100'0000'0000
#define SHADER_FLAG_TRIPLANER_SHARE_X_Z			0b0000'1000'0000'0000
#define SHADER_FLAG_TRIPLANER_SYNC_XZ			0b0001'0000'0000'0000

#define SHADER_TEXDIFFUSE						"g_texDiffuse"
#define SHADER_TEXNORMAL						"g_texNormal"

#define SHADER_TILING_DIFFUSE					"g_fTilingDiffuse"
#define SHADER_TILING_NORMAL					"g_fTilingNormal"

#define SHADER_MATWORLD							"g_mWorld"
#define SHADER_MATVIEW							"g_mView"
#define SHADER_MATPROJ							"g_mProj"
#define SHADER_CAMPOS							"g_vCamPosition"

#define SHADER_LIGHTCNT							"g_iLightCount"
#define SHADER_LIGHTTYPE						"g_iLightType"
#define SHADER_LIGHTPOS							"g_vLightPosition"
#define SHADER_LIGHTDIR							"g_vLightDirection"
#define SHADER_LIGHTDIF							"g_vLightDiffuse"
#define SHADER_LIGHTAMB							"g_vLightAmbient"
#define SHADER_LIGHTSPC							"g_vLightSpecular"
#define SHADER_LIGHTRNG							"g_fLightRange"
#define SHADER_ATT0								"g_fLightAttenuation0"
#define SHADER_ATT1								"g_fLightAttenuation1"
#define SHADER_ATT2								"g_fLightAttenuation2"

#define SHADER_MTRLDIF							"g_vMtrlDiffuse"
#define SHADER_MTRLAMB							"g_vMtrlAmbient"
#define SHADER_MTRLSPC							"g_vMtrlSpecular"
#define SHADER_MTRLEMS							"g_vMtrlEmissive"
#define SHADER_MTRLSHN							"g_fMtrlShininess"

#define SHADER_BONE								"g_mBones"

#define SHADER_TRIPLANER_SHARP					"g_fTPSharpness"
