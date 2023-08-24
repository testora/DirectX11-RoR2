#define	MAX_LIGHT			32
#define MAX_BONE			256

#define DIRECTIONAL			0
#define POINT				1

#define STATUS_TEXDIFFUSE	0x01
#define STATUS_TEXNORMAL	0x02

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

int			g_iFlag;

Texture2D	g_texDiffuse[8];
Texture2D	g_texNormal[8];

matrix		g_mWorld, g_mView, g_mProj;
vector		g_vCamPosition;

matrix		g_mBones				[MAX_BONE];

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

vector		g_vMtrlDiffuse			= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient			= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular			= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlEmissive			= vector(0.f, 0.f, 0.f, 0.f);
float		g_fMtrlShininess		= 32.f;

float		g_fTiling				= 0.02f;
float		g_fTPSharpness			= 1.f;

float4 TriPlanar_Diffuse(Texture2D texX, Texture2D texY, Texture2D texZ, float3 vBlendFactor, float3 vWorldPos)
{
	float4 vColorX			= texX.Sample(LinearSampler, vWorldPos.zy * g_fTiling);
	float4 vColorY			= texY.Sample(LinearSampler, vWorldPos.zx * g_fTiling);
	float4 vColorZ			= texZ.Sample(LinearSampler, vWorldPos.xy * g_fTiling);
	
	float4 vFinalColor		= vColorX * vBlendFactor.x + vColorY * vBlendFactor.y + vColorZ * vBlendFactor.z;
	
	return vFinalColor;
}
float4 TriPlanar_Diffuse(Texture2D texXZ, Texture2D texY, float3 vBlendFactor, float3 vWorldPos)
{
	float4 vColorXZ			= texXZ.Sample(LinearSampler, vWorldPos.xy * g_fTiling);
	float4 vColorY			= texY.Sample(LinearSampler, vWorldPos.xz * g_fTiling);
	
	float4 vFinalColor		= vColorXZ * (vBlendFactor.x + vBlendFactor.z) + vColorY * vBlendFactor.y;
	
	return vFinalColor;
}

float3 TriPlanar_Normal(Texture2D texX, Texture2D texY, Texture2D texZ, float3x3 vTBN, float3 vBlendFactor, float3 vWorldPos)
{
	float3 vNormalMapX		= texX.Sample(LinearSampler, vWorldPos.zy * g_fTiling).xyz * 2.f - 1.f;
	float3 vNormalMapY		= texY.Sample(LinearSampler, vWorldPos.zx * g_fTiling).xyz * 2.f - 1.f;
	float3 vNormalMapZ		= texZ.Sample(LinearSampler, vWorldPos.xy * g_fTiling).xyz * 2.f - 1.f;
	
	float3 vFinalNormalMap	= vNormalMapX * vBlendFactor.x + vNormalMapY * vBlendFactor.y + vNormalMapZ * vBlendFactor.z;
	float3 vFinalNormal		= mul(vFinalNormalMap, vTBN);

	return vFinalNormal;
}
float3 TriPlanar_Normal(Texture2D texXZ, Texture2D texY, float3x3 vTBN, float3 vBlendFactor, float3 vWorldPos)
{
	float3 vNormalMapXZ		= texXZ.Sample(LinearSampler, vWorldPos.xy * g_fTiling).xyz * 2.f - 1.f;
	float3 vNormalMapY		= texY.Sample(LinearSampler, vWorldPos.xz * g_fTiling).xyz * 2.f - 1.f;
	
	float3 vFinalNormalMap	= vNormalMapXZ * (vBlendFactor.x + vBlendFactor.z) + vNormalMapY * vBlendFactor.y;
	float3 vFinalNormal		= mul(vFinalNormalMap, vTBN);

	return vFinalNormal;
}

struct VS_IN
{
	float3	vPosition		: POSITION;
	float3	vNormal			: NORMAL;
	float2	vTexCoord		: TEXCOORD0;
	float3	vTangent		: TANGENT;
	uint4	vBlendIndices	: BLENDINDEX;
	float4	vBlendWeights	: BLENDWEIGHT;
};

struct VS_OUT
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: NORMAL;
	float4	vTangent		: TANGENT;
	float2	vTexCoord		: TEXCOORD0;
	float4	vWorldPos		: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;
	
	matrix	mWV, mWVP;

	mWV		= mul(g_mWorld, g_mView);
	mWVP	= mul(mWV, g_mProj);
	
	matrix mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
	vector vPosition	= mul(vector(In.vPosition, 1.f), mBone);
	vector vNormal		= mul(vector(normalize(In.vNormal), 0.f), mBone);
	vector vTangent		= mul(vector(normalize(In.vTangent), 0.f), mBone);

	Out.vPosition		= mul(vPosition, mWVP);
	Out.vNormal			= mul(vNormal, g_mWorld);
	Out.vTangent		= mul(vTangent, g_mWorld);
	Out.vTexCoord		= In.vTexCoord;
	Out.vWorldPos		= mul(vPosition, g_mWorld);

	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float4	vTangent	: TANGENT;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor		: SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out;
	
	float3		vFinalColor;
	float4		vTexColor	= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
	// Normal
	float3		vNormal		= In.vNormal.xyz;
	
    if (g_iFlag & STATUS_TEXNORMAL)
    {
		float3x3	vTBN		= float3x3(In.vTangent.xyz, cross(In.vNormal.xyz, In.vTangent.xyz), In.vNormal.xyz);
		float3		vNormalMap	= g_texNormal[0].Sample(LinearSampler, In.vTexCoord).xyz * 2.f - 1.f;
		vNormal					= mul(vNormalMap, vTBN);
    }
	
	// Specular
	float3		vViewDir	= normalize(g_vCamPosition - In.vWorldPos).xyz;
	
	for (int i = 0; i < g_iLightCount; ++i)
	{
		// Lighting
        float3	vLightDir	= float3(0.f, 0.f, 0.f);
		switch (g_iLightType[i])
		{
		case POINT:
            vLightDir		= normalize(In.vWorldPos - g_vLightDirection[i]).xyz;
			break;
		case DIRECTIONAL:
            vLightDir		= normalize(g_vLightDirection[i]).xyz;
            break;
        }
		
		float	fDiffuse	= saturate(dot(-vLightDir, vNormal));
		
		// Specular
        float3	vHalfDir	= normalize(vViewDir - vLightDir);
        float	fSpc		= pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
		
		// Attenuation
        float	fSmoothAtt	= 1.f;
        if (POINT == g_iLightType[i])
        {
            float	fDist		= distance(g_vLightPosition[i], In.vWorldPos);
            float	fAtt		= 1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
            float	fLightDist	= 1.f - saturate(fDist / g_fLightRange[i]);
            fSmoothAtt			= smoothstep(0.f, 1.f, fAtt * fLightDist);
        }
		
		// Final
        vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * fDiffuse + g_vLightAmbient[i].rgb)
							+	fSpc * g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb;
		
		vFinalColor			*=	fSmoothAtt;
    }
	
	// Final
	vFinalColor			+=	g_vMtrlEmissive.rgb;
	
	// Out
    Out.vColor.rgb		=	vFinalColor;
    Out.vColor.a		=	vTexColor.a;

	return Out;
}

PS_OUT PS_TRIPLANER_X_Y_Z(PS_IN In)
{
	PS_OUT		Out;
	
	// TriPlaner
	float3	vWeights		=	abs(In.vNormal.xyz);
	vWeights				/=	dot(vWeights, 1.f);

	float3	vBlendFactor	=	pow(vWeights, g_fTPSharpness);
	vBlendFactor			/=	dot(vBlendFactor, 1.0);

	float3		vFinalColor;
	float4		vTexColor	=	TriPlanar_Diffuse(g_texDiffuse[0], g_texDiffuse[1], g_texDiffuse[2], vBlendFactor, In.vWorldPos.xyz);
	
	// Normal
	float3		vNormal		=	In.vNormal.xyz;
	
	if (g_iFlag & STATUS_TEXNORMAL)
	{
		float3x3	vTBN	= float3x3(In.vTangent.xyz, cross(In.vNormal.xyz, In.vTangent.xyz), In.vNormal.xyz);
        vNormal				= TriPlanar_Normal(g_texNormal[0], g_texNormal[1], g_texNormal[2], vTBN, vBlendFactor, In.vWorldPos.xyz);
    }
	
	// Specular
	float3		vViewDir	= normalize(g_vCamPosition - In.vWorldPos).xyz;
	
	for (int i = 0; i < g_iLightCount; ++i)
	{
		// Lighting
		float3	vLightDir	= float3(0.f, 0.f, 0.f);
		switch (g_iLightType[i])
		{
		case POINT:
			vLightDir		= normalize(In.vWorldPos - g_vLightDirection[i]).xyz;
			break;
		case DIRECTIONAL:
			vLightDir		= normalize(g_vLightDirection[i]).xyz;
			break;
		}
		
		float	fDiffuse	= saturate(dot(-vLightDir, vNormal));
		
		// Specular
		float3	vHalfDir	= normalize(vViewDir - vLightDir);
		float	fSpc		= pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
		
		// Attenuation
		float	fSmoothAtt	= 1.f;
		if (POINT == g_iLightType[i])
		{
			float	fDist		= distance(g_vLightPosition[i], In.vWorldPos);
			float	fAtt		= 1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
			float	fLightDist	= 1.f - saturate(fDist / g_fLightRange[i]);
			fSmoothAtt			= smoothstep(0.f, 1.f, fAtt * fLightDist);
		}
		
		// Final
		vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * fDiffuse + g_vLightAmbient[i].rgb)
							+	fSpc * g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb;
		
		vFinalColor			*=	fSmoothAtt;
	}
	
	// Final
	vFinalColor			+=	g_vMtrlEmissive.rgb;
	
	// Out
	Out.vColor.rgb		=	vFinalColor;
	Out.vColor.a		=	vTexColor.a;

	return Out;
}

PS_OUT PS_TRIPLANER_XZ_Y(PS_IN In)
{
	PS_OUT		Out;
	
	// TriPlaner
	float3	vWeights		=	abs(In.vNormal.xyz);
	vWeights				/=	dot(vWeights, 1.f);

	float3	vBlendFactor	=	pow(vWeights, g_fTPSharpness);
	vBlendFactor			/=	dot(vBlendFactor, 1.0);

	float3	vFinalColor;
	float4	vTexColor		=	TriPlanar_Diffuse(g_texDiffuse[0], g_texDiffuse[1], vBlendFactor, In.vWorldPos.xyz);
	
	// Normal
	float3	vNormal			=	In.vNormal.xyz;
	
	if (g_iFlag & STATUS_TEXNORMAL)
	{
		float3x3	vTBN	= float3x3(In.vTangent.xyz, cross(In.vNormal.xyz, In.vTangent.xyz), In.vNormal.xyz);
        vNormal				= TriPlanar_Normal(g_texNormal[0], g_texNormal[1], vTBN, vBlendFactor, In.vWorldPos.xyz);
    }
	
	// Specular
	float3		vViewDir	= normalize(g_vCamPosition - In.vWorldPos).xyz;
	
	for (int i = 0; i < g_iLightCount; ++i)
	{
		// Lighting
		float3	vLightDir	= float3(0.f, 0.f, 0.f);
		switch (g_iLightType[i])
		{
		case POINT:
			vLightDir		= normalize(In.vWorldPos - g_vLightDirection[i]).xyz;
			break;
		case DIRECTIONAL:
			vLightDir		= normalize(g_vLightDirection[i]).xyz;
			break;
		}
		
		float	fDiffuse	= saturate(dot(-vLightDir, vNormal));
		
		// Specular
		float3	vHalfDir	= normalize(vViewDir - vLightDir);
		float	fSpc		= pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
		
		// Attenuation
		float	fSmoothAtt	= 1.f;
		if (POINT == g_iLightType[i])
		{
			float	fDist		= distance(g_vLightPosition[i], In.vWorldPos);
			float	fAtt		= 1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
			float	fLightDist	= 1.f - saturate(fDist / g_fLightRange[i]);
			fSmoothAtt			= smoothstep(0.f, 1.f, fAtt * fLightDist);
		}
		
		// Final
		vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * fDiffuse + g_vLightAmbient[i].rgb)
							+	fSpc * g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb;
		
		vFinalColor			*=	fSmoothAtt;
	}
	
	// Final
	vFinalColor			+=	g_vMtrlEmissive.rgb;
	
	// Out
	Out.vColor.rgb		=	vFinalColor;
	Out.vColor.a		=	vTexColor.a;

	return Out;
}

technique11 DefaultTechnique
{
	pass MESH
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}

	pass MESH_TRIPLANER_X_Y_Z
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_TRIPLANER_X_Y_Z();
	}

	pass MESH_TRIPLANER_XZ_Y
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_TRIPLANER_XZ_Y();
	}
}