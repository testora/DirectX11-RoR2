#include "ShaderDefine.hlsli"

int			g_iShaderFlag;

float4x4	g_mWorld, g_mView, g_mProj, g_mOrthographic;
float4x4	g_mViewInv, g_mProjInv;
float4		g_vCamPosition;
float		g_fCamFar				= 10000.f;

Texture2D	g_texDiffuse			[MAX_TEXTURE];
Texture2D	g_texNormal				[MAX_TEXTURE];
float		g_fTilingDiffuse		[MAX_TEXTURE];
float		g_fTilingNormal			[MAX_TEXTURE];

int			g_iLightCount			= 1;
int			g_iLightType;			//	[MAX_LIGHT];
float4		g_vLightPosition;		//	[MAX_LIGHT];
float4		g_vLightDirection;		//	[MAX_LIGHT];
float4		g_vLightDiffuse;		//	[MAX_LIGHT];
float4		g_vLightAmbient;		//	[MAX_LIGHT];
float4		g_vLightSpecular;		//	[MAX_LIGHT];
float		g_fLightRange;			//	[MAX_LIGHT];
float		g_fLightAttenuation0;	//	[MAX_LIGHT];
float		g_fLightAttenuation1;	//	[MAX_LIGHT];
float		g_fLightAttenuation2;	//	[MAX_LIGHT];

float4x4	g_mBones				[MAX_BONE];

float4		g_vMtrlDiffuse;
float4		g_vMtrlAmbient;
float4		g_vMtrlSpecular;
float4		g_vMtrlEmissive;
float		g_fMtrlShininess		= 32.f;

float		g_fTPSharpness			= 32.f;

float3		g_vSize					= float3(1.f, 1.f, 1.f);
float		g_fThickness			= 1.f;
int			g_iMaxInstance			= 100;

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
	
	if (vNormal.x > 0)
	{
		if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_X)
		{
			vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texDiffuse[TRIPLANAR_SLOT + 0].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 1.5f)	* g_fTilingDiffuse[TRIPLANAR_SLOT + 0]) * vBlendFactor.x:
								g_texDiffuse[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.yz							* g_fTilingDiffuse[TRIPLANAR_SLOT + 0]) * vBlendFactor.x;
			fBlendFactor	-=	vBlendFactor.x;
		}
    }
	if (vNormal.y > 0)
	{
		if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Y)
		{
			vTriColor		+=	g_texDiffuse[TRIPLANAR_SLOT + 1].Sample(LinearSampler, vWorldPos.zx							* g_fTilingDiffuse[TRIPLANAR_SLOT + 1]) * vBlendFactor.y;
			fBlendFactor	-=	vBlendFactor.y;
		}
    }
	if (vNormal.z > 0)
	{
		if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Z)
		{
			vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texDiffuse[TRIPLANAR_SLOT + 0].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)			* g_fTilingDiffuse[TRIPLANAR_SLOT + 0]) * vBlendFactor.z:
								g_texDiffuse[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.xy							* g_fTilingDiffuse[TRIPLANAR_SLOT + 0]) * vBlendFactor.z:
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texDiffuse[TRIPLANAR_SLOT + 2].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)			* g_fTilingDiffuse[TRIPLANAR_SLOT + 2]) * vBlendFactor.z:
								g_texDiffuse[TRIPLANAR_SLOT + 2].Sample(LinearSampler, vWorldPos.xy							* g_fTilingDiffuse[TRIPLANAR_SLOT + 2]) * vBlendFactor.z;
			fBlendFactor	-=	vBlendFactor.z;
		}
    }
	if (vNormal.x < 0)
	{
		if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_X)
		{
			vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X ?
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texDiffuse[TRIPLANAR_SLOT + 0].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingDiffuse[TRIPLANAR_SLOT + 0]) * vBlendFactor.x:
								g_texDiffuse[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.yz							* g_fTilingDiffuse[TRIPLANAR_SLOT + 0]) * vBlendFactor.x:
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texDiffuse[TRIPLANAR_SLOT + 3].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingDiffuse[TRIPLANAR_SLOT + 3]) * vBlendFactor.x:
								g_texDiffuse[TRIPLANAR_SLOT + 3].Sample(LinearSampler, vWorldPos.yz							* g_fTilingDiffuse[TRIPLANAR_SLOT + 3]) * vBlendFactor.x;
			fBlendFactor	-=	vBlendFactor.x;
		}
    }
	if (vNormal.y < 0)
	{
		if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Y)
		{
			vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Y ?
								g_texDiffuse[TRIPLANAR_SLOT + 1].Sample(LinearSampler, vWorldPos.zx							* g_fTilingDiffuse[TRIPLANAR_SLOT + 1]) * vBlendFactor.y:
								g_texDiffuse[TRIPLANAR_SLOT + 4].Sample(LinearSampler, vWorldPos.zx							* g_fTilingDiffuse[TRIPLANAR_SLOT + 4]) * vBlendFactor.y;
			fBlendFactor	-=	vBlendFactor.y;
		}
    }
	if (vNormal.z < 0)
	{
		if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Z)
		{
			vTriColor		+=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Z ?
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_texDiffuse[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.xy							* g_fTilingDiffuse[TRIPLANAR_SLOT + 0]) * vBlendFactor.z:
								g_texDiffuse[TRIPLANAR_SLOT + 2].Sample(LinearSampler, vWorldPos.xy							* g_fTilingDiffuse[TRIPLANAR_SLOT + 2]) * vBlendFactor.z:
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_texDiffuse[TRIPLANAR_SLOT + 3].Sample(LinearSampler, vWorldPos.xy							* g_fTilingDiffuse[TRIPLANAR_SLOT + 3]) * vBlendFactor.z:
								g_texDiffuse[TRIPLANAR_SLOT + 5].Sample(LinearSampler, vWorldPos.xy							* g_fTilingDiffuse[TRIPLANAR_SLOT + 5]) * vBlendFactor.z;
			fBlendFactor	-=	vBlendFactor.z;
		}
	}
	
	return vTriColor + vTexColor * fBlendFactor;
}

float3 TriPlanar_Mix_Normal(float3 vNormal, float3 vTangent, float3 vWorldPos, float3 vBlendFactor)
{
	float3x3	mTBN			= float3x3(vTangent, normalize(cross(vNormal, vTangent)), vNormal);
	float3		vTriNormal		= float3(0.f, 0.f, 0.f);
	float		fBlendFactor	= 1.f;
	
	if (vNormal.x > 0)
	{
        if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_X)
        {
			float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texNormal[TRIPLANAR_SLOT + 0].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 1.5f)	* g_fTilingNormal[TRIPLANAR_SLOT + 0]):
								g_texNormal[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.yz						* g_fTilingNormal[TRIPLANAR_SLOT + 0]);
			vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)														* vBlendFactor.x;
			fBlendFactor	-=	vBlendFactor.x;
        }
    }
	if (vNormal.y > 0)
	{
        if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Y)
        {
			float4 vDXT5nm	=	g_texNormal[TRIPLANAR_SLOT + 1].Sample(LinearSampler, vWorldPos.zx						* g_fTilingNormal[TRIPLANAR_SLOT + 1]);
			vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)														* vBlendFactor.y;
			fBlendFactor	-=	vBlendFactor.y;
        }
    }
	if (vNormal.z > 0)
	{
        if (g_iShaderFlag & STATUS_TRIPLANER_POSITIVE_Z)
        {
			float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texNormal[TRIPLANAR_SLOT + 0].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)		* g_fTilingNormal[TRIPLANAR_SLOT + 0]):
								g_texNormal[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[TRIPLANAR_SLOT + 0]):
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texNormal[TRIPLANAR_SLOT + 2].Sample(LinearSampler, RotateUV(vWorldPos.xy, PI)		* g_fTilingNormal[TRIPLANAR_SLOT + 2]):
								g_texNormal[TRIPLANAR_SLOT + 2].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[TRIPLANAR_SLOT + 2]);
			vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)														* vBlendFactor.z;
			fBlendFactor	-=	vBlendFactor.z;
        }
    }
	if (vNormal.x < 0)
	{
        if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_X)
        {
			float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_X ?
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texNormal[TRIPLANAR_SLOT + 0].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingNormal[TRIPLANAR_SLOT + 0]):
								g_texNormal[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.yz						* g_fTilingNormal[TRIPLANAR_SLOT + 0]):
								g_iShaderFlag & STATUS_TRIPLANER_SYNC_XZ ?
								g_texNormal[TRIPLANAR_SLOT + 3].Sample(LinearSampler, RotateUV(vWorldPos.yz, PI * 0.5f)	* g_fTilingNormal[TRIPLANAR_SLOT + 3]):
								g_texNormal[TRIPLANAR_SLOT + 3].Sample(LinearSampler, vWorldPos.yz						* g_fTilingNormal[TRIPLANAR_SLOT + 3]);
			vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)														* vBlendFactor.x;
			fBlendFactor	-=	vBlendFactor.x;
        }
    }
	if (vNormal.y < 0)
	{
        if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Y)
        {
			float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Y ?
								g_texNormal[TRIPLANAR_SLOT + 1].Sample(LinearSampler, vWorldPos.zx						* g_fTilingNormal[TRIPLANAR_SLOT + 1]):
								g_texNormal[TRIPLANAR_SLOT + 4].Sample(LinearSampler, vWorldPos.zx						* g_fTilingNormal[TRIPLANAR_SLOT + 4]);
			vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)														* vBlendFactor.y;
			fBlendFactor	-=	vBlendFactor.y;
        }
    }
	if (vNormal.z < 0)
	{
        if (g_iShaderFlag & STATUS_TRIPLANER_NEGATIVE_Z)
        {
			float4 vDXT5nm	=	g_iShaderFlag & STATUS_TRIPLANER_SHARE_Z ?
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_texNormal[TRIPLANAR_SLOT + 0].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[TRIPLANAR_SLOT + 0]):
								g_texNormal[TRIPLANAR_SLOT + 2].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[TRIPLANAR_SLOT + 2]):
								g_iShaderFlag & STATUS_TRIPLANER_SHARE_X_Z ?
								g_texNormal[TRIPLANAR_SLOT + 3].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[TRIPLANAR_SLOT + 3]):
								g_texNormal[TRIPLANAR_SLOT + 5].Sample(LinearSampler, vWorldPos.xy						* g_fTilingNormal[TRIPLANAR_SLOT + 5]);
			vTriNormal		+=	UnpackNormalDXT5nm(vDXT5nm, mTBN)														* vBlendFactor.z;
			fBlendFactor	-=	vBlendFactor.z;
        }
    }
	
	return vTriNormal + vNormal * fBlendFactor;
}
