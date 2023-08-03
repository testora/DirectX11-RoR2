matrix		g_mWorld, g_mView, g_mProj;
texture2D	g_texDiffuse;

sampler		LinearSampler	= sampler_state
{
	Filter	= MIN_MAG_MIP_LINEAR;
};

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

    mWV		= mul(g_mWorld, g_mView);
    mWVP	= mul(mWV, g_mProj);

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