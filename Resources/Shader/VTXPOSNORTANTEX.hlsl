#define	MAX_LIGHT	32
#define MAX_BONE	256

#define POINT		1
#define DIRECTIONAL	2

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

Texture2D	g_texDiffuse;

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

float	    g_fLightSpecularPower	= 16.f;

vector		g_vMtrlDiffuse			= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient			= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular			= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlEmissive			= vector(0.f, 0.f, 0.f, 0.f);
float		g_fMtrlShininess		= 30.f;

struct VS_IN
{
	float3 vPosition	: POSITION;
	float3 vNormal		: NORMAL;
	float2 vTexCoord	: TEXCOORD0;
	float3 vTangent		: TANGENT;
};

struct VS_OUT
{
	float4 vPosition	: SV_POSITION;
	float4 vNormal		: NORMAL;
	float2 vTexCoord	: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	matrix	mWV, mWVP;

    mWV				= mul(g_mWorld, g_mView);
    mWVP			= mul(mWV, g_mProj);

    Out.vPosition	= mul(vector(In.vPosition, 1.f), mWVP);
    Out.vNormal		= mul(vector(In.vNormal, 0.f), g_mWorld);
	Out.vTexCoord	= In.vTexCoord;
    Out.vWorldPos	= mul(vector(In.vPosition, 1.f), g_mWorld);

	return Out;
}

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float4 vNormal		: NORMAL;
	float2 vTexCoord	: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out;
	
	float4	vTexColor	= g_texDiffuse.Sample(LinearSampler, In.vTexCoord);
	float3	vFinalColor;
	
	// Lighting
	float3	vNormal		= In.vNormal;
	
	// Specular
	float3	vViewDir	= normalize(g_vCamPosition - In.vWorldPos);
	
	for (int i = 0; i < g_iLightCount; ++i)
	{
		// Lighting
        float3	vLightDir;
		switch (g_iLightType[i])
		{
		case POINT:
            vLightDir		= normalize(In.vWorldPos - g_vLightDirection[i]);
			break;
		case DIRECTIONAL:
            vLightDir		= normalize(g_vLightDirection[i]);
            break;
        }
		
		float	fDiffuse	= saturate(dot(-vLightDir, vNormal));
		
		// Specular
        float3	vHalfDir	= normalize(vViewDir - vLightDir);
        float	fSpc		= pow(max(dot(vViewDir, vHalfDir), 0.f), g_fLightSpecularPower);
		
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
    Out.vColor.rgb		= vFinalColor;
    Out.vColor.a		= vTexColor.a;

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
}