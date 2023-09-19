#include "ShaderDefine.hlsli"

int			g_iShaderFlag;

matrix		g_mWorld, g_mView, g_mProj, g_mOrthographic;
vector		g_vCamPosition;

Texture2D	g_texDiffuse			[MAX_TEXTURE];
Texture2D	g_texNormal				[MAX_TEXTURE];
float		g_fTilingDiffuse		[MAX_TEXTURE];
float		g_fTilingNormal			[MAX_TEXTURE];

int			g_iLightCount			= 1;
int			g_iLightType			[MAX_LIGHT];
vector		g_vLightPosition		[MAX_LIGHT];
vector		g_vLightDirection		[MAX_LIGHT];
vector		g_vLightDiffuse			[MAX_LIGHT];
vector		g_vLightAmbient			[MAX_LIGHT];
vector		g_vLightSpecular		[MAX_LIGHT];
float		g_fLightRange			[MAX_LIGHT];
float		g_fLightAttenuation0	[MAX_LIGHT];
float		g_fLightAttenuation1	[MAX_LIGHT];
float		g_fLightAttenuation2	[MAX_LIGHT];

matrix		g_mBones				[MAX_BONE];

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;
vector		g_vMtrlEmissive;
float		g_fMtrlShininess		= 32.f;

float		g_fTPSharpness			= 2.f;

float AvgGrayScale(float3 vRGB)
{
    return (vRGB.r + vRGB.g + vRGB.b) / 3.f;
}

float2 RotateUV(float2 vUV, float fRadian)
{
	float fSin = sin(fRadian);
	float fCos = cos(fRadian);

	float2 vRotateUV;
	vRotateUV.x = vUV.x * fCos - vUV.y * fSin;
	vRotateUV.y = vUV.x * fSin + vUV.y * fCos;
	
	return vRotateUV;
}

float3 BlendFactor(float3 vNormal)
{
	float3	vWeights		=	abs(vNormal);
	vWeights				/=	dot(vWeights, 1.f);

	float3	vBlendFactor	=	pow(vWeights, g_fTPSharpness);
	vBlendFactor			/=	dot(vBlendFactor, 1.f);
	
	return vBlendFactor;
}

float3 UnpackNormalDXT5nm(float4 vPackedNormal, float3x3 mTBN)
{
	float3 vNormalMap;
	
	vNormalMap.rg	= vPackedNormal.ag * 2.f - 1.f;
	vNormalMap.b	= sqrt(1.f - saturate(dot(vNormalMap.rg, vNormalMap.rg)));
	
	return normalize(mul(vNormalMap, mTBN));
}

float4 TriPlanar_Mix_Diffuse(float4 vTexColor, float3 vNormal, float3 vWorldPos, float3 vBlendFactor)
{
	float4	vTriColor		= float4(0.f, 0.f, 0.f, 0.f);
	float   fBlendFactor	= 1.f;
	
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_X && vNormal.x > 0)
	{
		vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texDiffuse[1].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 1.5f)	* g_fTilingDiffuse[1]) * vBlendFactor.x:
							g_texDiffuse[1].Sample(LinearSampler, vWorldPos.yz						* g_fTilingDiffuse[1]) * vBlendFactor.x;
		fBlendFactor	-=	vBlendFactor.x;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Y && vNormal.y > 0)
	{
		vTriColor		+=	g_texDiffuse[2].Sample(LinearSampler, vWorldPos.zx						* g_fTilingDiffuse[2]) * vBlendFactor.y;
		fBlendFactor	-=	vBlendFactor.y;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Z && vNormal.z > 0)
	{
		vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texDiffuse[1].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)		* g_fTilingDiffuse[1]) * vBlendFactor.z:
							g_texDiffuse[1].Sample(LinearSampler, vWorldPos.xy						* g_fTilingDiffuse[1]) * vBlendFactor.z:
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texDiffuse[3].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)		* g_fTilingDiffuse[3]) * vBlendFactor.z:
							g_texDiffuse[3].Sample(LinearSampler, vWorldPos.xy						* g_fTilingDiffuse[3]) * vBlendFactor.z;
		fBlendFactor	-=	vBlendFactor.z;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_X && vNormal.x < 0)
	{
		vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X ?
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texDiffuse[1].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingDiffuse[1]) * vBlendFactor.x:
							g_texDiffuse[1].Sample(LinearSampler, vWorldPos.yz						* g_fTilingDiffuse[1]) * vBlendFactor.x:
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texDiffuse[4].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingDiffuse[4]) * vBlendFactor.x:
							g_texDiffuse[4].Sample(LinearSampler, vWorldPos.yz						* g_fTilingDiffuse[4]) * vBlendFactor.x;
		fBlendFactor	-=	vBlendFactor.x;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Y && vNormal.y < 0)
	{
		vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Y ?
							g_texDiffuse[2].Sample(LinearSampler, vWorldPos.zx						* g_fTilingDiffuse[2]) * vBlendFactor.y:
							g_texDiffuse[5].Sample(LinearSampler, vWorldPos.zx						* g_fTilingDiffuse[5]) * vBlendFactor.y;
		fBlendFactor	-=	vBlendFactor.y;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Z && vNormal.z < 0)
	{
		vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Z ?
							g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
							g_texDiffuse[1].Sample(LinearSampler, vWorldPos.xy						* g_fTilingDiffuse[1]) * vBlendFactor.z:
							g_texDiffuse[3].Sample(LinearSampler, vWorldPos.xy						* g_fTilingDiffuse[3]) * vBlendFactor.z:
							g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
							g_texDiffuse[4].Sample(LinearSampler, vWorldPos.xy						* g_fTilingDiffuse[4]) * vBlendFactor.z:
							g_texDiffuse[6].Sample(LinearSampler, vWorldPos.xy						* g_fTilingDiffuse[6]) * vBlendFactor.z;
		fBlendFactor	-=	vBlendFactor.z;
	}
	
	return vTriColor + vTexColor * fBlendFactor;
}

float3 TriPlanar_Mix_Normal(float3 vNormal, float3 vTangent, float3 vWorldPos, float3 vBlendFactor)
{
	float3x3	mTBN			= float3x3(vTangent, normalize(cross(vNormal, vTangent)), vNormal);
	float3		vTriNormal		= float3(0.f, 0.f, 0.f);
	float		fBlendFactor	= 1.f;
	
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_X && vNormal.x > 0)
	{
		float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texNormal[1].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 1.5f)	* g_fTilingNormal[1]):
							g_texNormal[1].Sample(LinearSampler, vWorldPos.yz						* g_fTilingNormal[1]);
		vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)										* vBlendFactor.x;
		fBlendFactor	-=	vBlendFactor.x;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Y && vNormal.y > 0)
	{
		float4 vDXT5nm	=	g_texNormal[2].Sample(LinearSampler, vWorldPos.zx						* g_fTilingNormal[2]);
		vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)										* vBlendFactor.y;
		fBlendFactor	-=	vBlendFactor.y;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Z && vNormal.z > 0)
	{
		float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texNormal[1].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)			* g_fTilingNormal[1]):
							g_texNormal[1].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[1]):
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texNormal[3].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)			* g_fTilingNormal[3]):
							g_texNormal[3].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[3]);
		vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)										* vBlendFactor.z;
		fBlendFactor	-=	vBlendFactor.z;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_X && vNormal.x < 0)
	{
		float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X ?
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texNormal[1].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingNormal[1]):
							g_texNormal[1].Sample(LinearSampler, vWorldPos.yz						* g_fTilingNormal[1]):
							g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
							g_texNormal[4].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingNormal[4]):
							g_texNormal[4].Sample(LinearSampler, vWorldPos.yz						* g_fTilingNormal[4]);
		vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)										* vBlendFactor.x;
		fBlendFactor	-=	vBlendFactor.x;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Y && vNormal.y < 0)
	{
		float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Y ?
							g_texNormal[2].Sample(LinearSampler, vWorldPos.zx						* g_fTilingNormal[2]):
							g_texNormal[5].Sample(LinearSampler, vWorldPos.zx						* g_fTilingNormal[5]);
		vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)										* vBlendFactor.y;
		fBlendFactor	-=	vBlendFactor.y;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Z && vNormal.z < 0)
	{
		float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Z ?
							g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
							g_texNormal[1].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[1]):
							g_texNormal[3].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[3]):
							g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
							g_texNormal[4].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[4]):
							g_texNormal[6].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[6]);
		vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)										* vBlendFactor.z;
		fBlendFactor	-=	vBlendFactor.z;
	}
	
	return vTriNormal + vNormal * fBlendFactor;
}
