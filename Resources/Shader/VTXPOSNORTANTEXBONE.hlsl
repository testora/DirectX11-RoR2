Texture2D	g_texDiffuse;

matrix		g_mWorld, g_mView, g_mProj;
vector		g_vCamPos;

float		g_fLightRange		= 5.f;
vector		g_vLightDir			= vector(1.f, -1.f, 1.f, 0.f);
vector		g_vLightPos			= vector(5.f, 3.f, 5.f, 1.f);
vector		g_vLightDiffuse		= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vLightAmbient		= vector(1.f, 1.f, 1.f, 1.f);
vector		g_vLightSpecular	= vector(1.f, 0.0f, 0.f, 1.f);

vector		g_vMtrlAmbient		= vector(0.4f, 0.4f, 0.4f, 1.f);
vector		g_vMtrlSpecular		= vector(1.f, 1.f, 1.f, 1.f);

matrix		g_mBones[256];

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
	
    vector vPosition = mul(vector(In.vPosition, 1.f), mBone);

    Out.vPosition	= mul(vPosition, mWVP);
    Out.vNormal		= mul(vector(In.vNormal, 0.f), g_mWorld);
	Out.vTexCoord	= In.vTexCoord;
    Out.vWorldPos	= mul(vPosition, g_mWorld);

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
	PS_OUT Out;

	vector vMtrlDiffuse = g_texDiffuse.Sample(LinearSampler, In.vTexCoord);

	float fShade = saturate(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)));

	vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));

	vector vLook = In.vWorldPos - g_vCamPos;

	float fSpecular = pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 30.0f);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

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