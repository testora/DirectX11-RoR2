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

Texture2D	g_texDiffuse;
Texture2D	g_texNormal;

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

struct VS_IN
{
	float3 vPosition	: POSITION;
	float2 vTexCoord	: TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	matrix	mWV, mWVP;

    mWV				= mul(g_mWorld, g_mView);
    mWVP			= mul(mWV, g_mProj);

    Out.vPosition	= mul(vector(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;

	return Out;
}

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor		: SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;

	Out.vColor	= g_texDiffuse.Sample(LinearSampler, In.vTexCoord);

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
}