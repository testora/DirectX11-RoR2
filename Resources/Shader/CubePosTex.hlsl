#include "ShaderCommon.hlsli"

TextureCube g_texCubeDiffuse;

struct VS_IN
{
	float3	vPosition	: POSITION;
	float3	vTexCoord	: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float3	vTexCoord	: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	matrix	mWV, mWVP;

	mWV				= mul(g_mWorld, g_mView);
	mWVP			= mul(mWV, g_mProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;

	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float3	vTexCoord	: TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor		: SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out;
	
	Out.vColor	=	g_texCubeDiffuse.Sample(LinearSampler, In.vTexCoord);
	Out.vColor	*=	g_vMtrlDiffuse;
	
	if (In.vTexCoord.y > 0.f)
	{
	    Out.vColor = lerp(Out.vColor, float4(0.95f, 0.95f, 0.95f, 1.f), smoothstep(0.f, 0.5f, In.vTexCoord.y) * 0.8f + 0.2f);
	}
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

    pass Sky
    {
        VertexShader	= compile vs_5_0 VS_MAIN();
        GeometryShader	= NULL;
        HullShader		= NULL;
        DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_InverseCull);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }
}
