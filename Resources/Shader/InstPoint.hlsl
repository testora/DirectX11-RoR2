#include "ShaderCommon.hlsli"

struct VS_IN
{
	float3 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
};

struct VS_OUT
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;
	
	float4x4			mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	float4				vPosition		= mul(float4(In.vPosition, 1.f), mTransformation);

	Out.vPosition		= mul(vPosition, g_mWorld);
	Out.vSize			= In.vSize;

	return Out;
}

struct GS_IN
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
};

struct GS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
    float4 a = float4(-1900, 1027, 710, 1);
	float4 vLook		= g_vCamPosition - In[0].vPosition;

	float3 vRight		= normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vSize.x * 0.5f;
	float3 vUp			= normalize(cross(vLook.xyz, vRight)) * In[0].vSize.y * 0.5f;

	matrix matVP		= mul(g_mView, g_mProj);

	Out[0].vPosition	= float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition	= mul(Out[0].vPosition, matVP);
	Out[0].vTexCoord	= float2(0.f, 0.f);

	Out[1].vPosition	= float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition	= mul(Out[1].vPosition, matVP);
	Out[1].vTexCoord	= float2(1.f, 0.f);

	Out[2].vPosition	= float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition	= mul(Out[2].vPosition, matVP);
	Out[2].vTexCoord	= float2(1.f, 1.f);

	Out[3].vPosition	= float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition	= mul(Out[3].vPosition, matVP);
	Out[3].vTexCoord	= float2(0.f, 1.f);

	Triangles.Append(Out[0]);
	Triangles.Append(Out[1]);
	Triangles.Append(Out[2]);
	Triangles.RestartStrip();

	Triangles.Append(Out[0]);
	Triangles.Append(Out[2]);
	Triangles.Append(Out[3]);
	Triangles.RestartStrip();
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

	Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= compile gs_5_0 GS_MAIN();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
}
