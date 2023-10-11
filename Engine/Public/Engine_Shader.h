#pragma once

#define SHADER_TIME								"g_fTime"

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

#define SHADER_CAM_FAR							"g_fCamFar"

#define SHADER_TEXDIFFUSE						"g_texDiffuse"
#define SHADER_TEXNORMAL						"g_texNormal"

#define SHADER_TILING_DIFFUSE					"g_fTilingDiffuse"
#define SHADER_TILING_NORMAL					"g_fTilingNormal"

#define SHADER_MATWORLD							"g_mWorld"
#define SHADER_MATVIEW							"g_mView"
#define SHADER_MATVIEWINV						"g_mViewInv"
#define SHADER_MATPROJ							"g_mProj"
#define SHADER_MATPROJINV						"g_mProjInv"
#define SHADER_MATORTHOGRAPHIC					"g_mOrthographic"
#define SHADER_CAMPOS							"g_vCamPosition"

#define SHADER_TEXTARGET_MTRL_DIFFUSE			"g_texMtrlDiffuseTarget"
#define SHADER_TEXTARGET_MTRL_AMBIENT			"g_texMtrlAmbientTarget"
#define SHADER_TEXTARGET_MTRL_SPECULAR			"g_texMtrlSpecularTarget"
#define SHADER_TEXTARGET_MTRL_EMISSIVE			"g_texMtrlEmissiveTarget"
#define SHADER_TEXTARGET_NORMAL					"g_texNormalTarget"
#define SHADER_TEXTARGET_DEPTH					"g_texDepthTarget"
#define SHADER_TEXTARGET_SHADE					"g_texShadeTarget"
#define SHADER_TEXTARGET_SPECULAR				"g_texSpecularTarget"
#define SHADER_TEXTARGET_PREPROCESS				"g_texPreProcessTarget"
#define SHADER_TEXTARGET_MASK					"g_texMaskTarget"

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

#define SHADER_SIZE								"g_vSize"
#define SHADER_THICKNESS						"g_fThickness"
#define SHADER_MAXINSTANCE						"g_iMaxInstance"

#define RENDERTARGET_MATERIAL_DIFFUSE			TEXT("RenderTarget:Material:Diffuse")
#define RENDERTARGET_MATERIAL_AMBIENT			TEXT("RenderTarget:Material:Ambient")
#define RENDERTARGET_MATERIAL_SPECULAR			TEXT("RenderTarget:Material:Specular")
#define RENDERTARGET_MATERIAL_EMISSIVE			TEXT("RenderTarget:Material:Emissive")

#define RENDERTARGET_NORMAL						TEXT("RenderTarget:Normal")
#define RENDERTARGET_DEPTH						TEXT("RenderTarget:Depth")

#define RENDERTARGET_SHADE						TEXT("RenderTarget:Shade")
#define RENDERTARGET_SPECULAR					TEXT("RenderTarget:Specular")

#define RENDERTARGET_PREPROCESS					TEXT("RenderTarget:PreProcess")
#define RENDERTARGET_MASK						TEXT("RenderTarget:Mask")

#define MULTIRENDERTARGET_NONBLEND				TEXT("MultiRenderTarget:NonBlend")
#define MULTIRENDERTARGET_LIGHT					TEXT("MultiRenderTarget:Light")
#define MULTIRENDERTARGET_POSTPROCESS			TEXT("MultiRenderTarget:PostProcess")
