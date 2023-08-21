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
};

struct VS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out;

	matrix			mWV, mWVP;

	mWV				= mul(g_mWorld, g_mView);
	mWVP			= mul(mWV, g_mProj);

	Out.vPosition	= mul(vector(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(vector(In.vPosition, 1.f), g_mWorld);

	return Out;
}

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vWorldPos	: TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor		: SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;

	Out.vColor	= g_texDiffuse.Sample(LinearSampler, In.vTexCoord * 30.f);

	return Out;
}

PS_OUT PS_BRUSH(PS_IN In)
{
	PS_OUT Out;

	Out.vColor	= g_texDiffuse.Sample(LinearSampler, In.vTexCoord * 30.f);
	
//	for (int i = 0; i < g_iBrushCount; ++i)
//	{
//		vector vBrush = vector(0.f, 0.f, 0.f, 0.f);
//		
//		if (g_vBrushPos[i].x - g_fRadius < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos[i].x + g_fRadius
//		&&	g_vBrushPos[i].z - g_fRadius < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos[i].z + g_fRadius)
//		{
//			float2 vTexcoord;
//
//			vTexcoord.x = (In.vWorldPos.x - (g_vBrushPos[i].x - g_fRadius)) / (2.f * g_fRadius);
//			vTexcoord.y = ((g_vBrushPos[i].z + g_fRadius) - In.vWorldPos.z) / (2.f * g_fRadius);
//
//			vBrush = g_texBrush.Sample(LinearSampler, vTexcoord);
//		}
//		
//		Out.vColor += vBrush;
//	}

	return Out;
}

technique11 DefaultTechnique
{
	pass DEFAULT
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}

	pass BRUSH
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_BRUSH();
	}
}