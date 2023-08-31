#define MAX_TEXTURE						8
#define	MAX_LIGHT						32
#define MAX_BONE						256
#define PI								3.141592654f

#define DIRECTIONAL						0
#define POINT							1

#define STATUS_TEXDIFFUSE				0x0001
#define STATUS_TEXNORMAL				0x0002
#define STATUS_TRIPLANER_POSITIVE_X		0x0004
#define STATUS_TRIPLANER_POSITIVE_Y		0x0008
#define STATUS_TRIPLANER_POSITIVE_Z		0x0010
#define STATUS_TRIPLANER_NEGATIVE_X		0x0020
#define STATUS_TRIPLANER_NEGATIVE_Y		0x0040
#define STATUS_TRIPLANER_NEGATIVE_Z		0x0080
#define STATUS_TRIPLANER_SHARE_X        0x0100
#define STATUS_TRIPLANER_SHARE_Y        0x0200
#define STATUS_TRIPLANER_SHARE_Z        0x0400
#define STATUS_TRIPLANER_SHARE_X_Z      0x0800
#define STATUS_TRIPLANER_SYNC_XZ		0x1000

sampler LinearSampler = sampler_state
{
	Filter		= MIN_MAG_MIP_LINEAR;
	AddressU	= wrap;
	AddressV	= wrap;
};

sampler PointSampler = sampler_state
{
	Filter		= MIN_MAG_MIP_POINT;
	AddressU	= wrap;
	AddressV	= wrap;
};

int			g_iShaderFlag;

matrix		g_mWorld, g_mView, g_mProj;
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
		vTriColor		+=	g_texDiffuse[2].Sample(LinearSampler, vWorldPos.zx * g_fTilingDiffuse[2]) * vBlendFactor.y;
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
							g_texDiffuse[2].Sample(LinearSampler, vWorldPos.zx * g_fTilingDiffuse[2]) * vBlendFactor.y:
							g_texDiffuse[5].Sample(LinearSampler, vWorldPos.zx * g_fTilingDiffuse[5]) * vBlendFactor.y;
		fBlendFactor	-=	vBlendFactor.y;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Z && vNormal.z < 0)
    {
        true ? true ? 1 : 1 : true ? 1 : 1;
		vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Z ?
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_texDiffuse[1].Sample(LinearSampler, vWorldPos.xy * g_fTilingDiffuse[1]) * vBlendFactor.z:
								g_texDiffuse[3].Sample(LinearSampler, vWorldPos.xy * g_fTilingDiffuse[3]) * vBlendFactor.z:
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_texDiffuse[4].Sample(LinearSampler, vWorldPos.xy * g_fTilingDiffuse[4]) * vBlendFactor.z:
								g_texDiffuse[6].Sample(LinearSampler, vWorldPos.xy * g_fTilingDiffuse[6]) * vBlendFactor.z;
		fBlendFactor	-=	vBlendFactor.z;
	}
	
	return vTriColor + vTexColor * fBlendFactor;
}

float3 TriPlanar_Mix_Normal(float3 vNormal, float3 vTangent, float3 vWorldPos, float3 vBlendFactor)
{
	float3x3	mTBN			= float3x3(vTangent, cross(vNormal, vTangent), vNormal);
	float3		vTriNormal		= float3(0.f, 0.f, 0.f);
	float		fBlendFactor	= 1.f;
	
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_X && vNormal.x > 0)
	{
		vTriNormal		+=	(g_texNormal[1].Sample(LinearSampler, vWorldPos.yz * g_fTilingNormal[1]).xyz * 2.f - 1.f) * vBlendFactor.x;
		fBlendFactor	-=	vBlendFactor.x;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Y && vNormal.y > 0)
	{
		vTriNormal		+=	(g_texNormal[2].Sample(LinearSampler, vWorldPos.zx * g_fTilingNormal[2]).xyz * 2.f - 1.f) * vBlendFactor.y;
		fBlendFactor	-=	vBlendFactor.y;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Z && vNormal.z > 0)
	{
		vTriNormal		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
							(g_texNormal[1].Sample(LinearSampler, vWorldPos.xy * g_fTilingNormal[1]).xyz * 2.f - 1.f) * vBlendFactor.z:
							(g_texNormal[3].Sample(LinearSampler, vWorldPos.xy * g_fTilingNormal[3]).xyz * 2.f - 1.f) * vBlendFactor.z;
		fBlendFactor	-=	vBlendFactor.z;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_X && vNormal.x < 0)
	{
		vTriNormal		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X ?
							(g_texNormal[1].Sample(LinearSampler, vWorldPos.yz * g_fTilingNormal[1]).xyz * 2.f - 1.f) * vBlendFactor.x:
							(g_texNormal[4].Sample(LinearSampler, vWorldPos.yz * g_fTilingNormal[4]).xyz * 2.f - 1.f) * vBlendFactor.x;
		fBlendFactor	-=	vBlendFactor.x;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Y && vNormal.y < 0)
	{
		vTriNormal		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Y ?
							(g_texNormal[2].Sample(LinearSampler, vWorldPos.zx * g_fTilingNormal[2]).xyz * 2.f - 1.f) * vBlendFactor.y:
							(g_texNormal[5].Sample(LinearSampler, vWorldPos.zx * g_fTilingNormal[5]).xyz * 2.f - 1.f) * vBlendFactor.y;
		fBlendFactor	-=	vBlendFactor.y;
	}
	if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Z && vNormal.z < 0)
	{
		vTriNormal		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Z ?
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								(g_texNormal[1].Sample(LinearSampler, vWorldPos.xy * g_fTilingNormal[1]).xyz * 2.f - 1.f) * vBlendFactor.z:
								(g_texNormal[3].Sample(LinearSampler, vWorldPos.xy * g_fTilingNormal[3]).xyz * 2.f - 1.f) * vBlendFactor.z:
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								(g_texNormal[4].Sample(LinearSampler, vWorldPos.xy * g_fTilingNormal[4]).xyz * 2.f - 1.f) * vBlendFactor.z:
								(g_texNormal[6].Sample(LinearSampler, vWorldPos.xy * g_fTilingNormal[6]).xyz * 2.f - 1.f) * vBlendFactor.z;
		fBlendFactor	-=	vBlendFactor.z;
	}
	
	return mul(vTriNormal + vNormal * fBlendFactor, mTBN);
}
