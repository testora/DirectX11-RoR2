#include "ShaderCommon.hlsli"

struct VS_IN
{
	float3 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vRight		: TEXCOORD1;
	float4 vUp			: TEXCOORD2;
	float4 vLook		: TEXCOORD3;
	float4 vTranslation	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
	float4 vArgument	: TEXCOORD6;
};

struct VS_OUT
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
};

struct VS_OUT_LINE
{
	float4 vPosition	: POSITION;
    float2 vSize		: TEXCOORD0;
	
	float4 vPrevious    : TEXCOORD1;
    float4 vStart		: TEXCOORD2;
    float4 vEnd			: TEXCOORD3;
    float4 vNext		: TEXCOORD4;
    float4 vColor		: TEXCOORD5;
	float4 vArgument	: TEXCOORD6;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out;
	
	float4x4			mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	float4				vPosition		= mul(float4(In.vPosition, 1.f), mTransformation);
	
	float4x4			mWV, mWVP;

	mWV					= mul(g_mWorld, g_mView);
	mWVP				= mul(mWV, g_mProj);

	Out.vPosition		= mul(vPosition, mWVP);
	Out.vSize			= In.vSize;
	Out.vColor			= In.vColor;

	return Out;
}

VS_OUT_LINE VS_MAIN_LINE(VS_IN In)
{
    VS_OUT_LINE Out;
	
	Out.vPosition		= float4(In.vRight + In.vUp) * 0.5f;
	Out.vSize			= In.vSize;
	
	Out.vPrevious		= In.vRight;
	Out.vStart			= In.vUp;
	Out.vEnd			= In.vLook;
	Out.vNext			= In.vTranslation;
	Out.vColor			= In.vColor;
	Out.vArgument		= In.vArgument;

	return Out;
}


struct GS_IN
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
};

struct GS_IN_LINE
{
	float4 vPosition	: POSITION;
    float2 vSize		: TEXCOORD0;
	
	float4 vPrevious    : TEXCOORD1;
    float4 vStart		: TEXCOORD2;
    float4 vEnd			: TEXCOORD3;
    float4 vNext		: TEXCOORD4;
    float4 vColor		: TEXCOORD5;
    float4 vArgument	: TEXCOORD6;
};

struct GS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
};

[maxvertexcount(6)]
void GS_MAIN(line GS_IN In[2], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
	
	Out[0].vPosition	= float4(In[0].vPosition.xyz + In[0].vSize.x + In[0].vSize.y, 1.f);
	Out[0].vTexCoord	= float2(0.f, 0.f);
	Out[0].vColor		= In[0].vColor;

	Out[1].vPosition	= float4(In[0].vPosition.xyz - In[0].vSize.x + In[0].vSize.y, 1.f);
	Out[1].vTexCoord	= float2(1.f, 0.f);
	Out[1].vColor		= In[0].vColor;

	Out[2].vPosition	= float4(In[0].vPosition.xyz - In[0].vSize.x - In[0].vSize.y, 1.f);
	Out[2].vTexCoord	= float2(1.f, 1.f);
	Out[2].vColor		= In[0].vColor;

	Out[3].vPosition	= float4(In[0].vPosition.xyz + In[0].vSize.x - In[0].vSize.y, 1.f);
	Out[3].vTexCoord	= float2(0.f, 1.f);
	Out[3].vColor		= In[0].vColor;

	Triangles.Append(Out[0]);
	Triangles.Append(Out[1]);
	Triangles.Append(Out[2]);
	Triangles.RestartStrip();

	Triangles.Append(Out[0]);
	Triangles.Append(Out[2]);
	Triangles.Append(Out[3]);
	Triangles.RestartStrip();
}

[maxvertexcount(6)]
void GS_MAIN_LINE(line GS_IN_LINE In[2], inout TriangleStream<GS_OUT> Triangles)
{
    GS_OUT Out[4];
	
	float4x4	mWV		= mul(g_mWorld, g_mView);
	float4x4	mWVP	= mul(mWV, g_mProj);
	
	float3	vLook		= normalize(In[0].vPosition - g_vCamPosition).xyz;
	
	float3	vPrevDir	= normalize(In[0].vStart - In[0].vPrevious).xyz;
	float3	vCurDir		= normalize(In[1].vEnd - In[0].vStart).xyz;
	float3	vNextDir	= normalize(In[1].vNext - In[0].vEnd).xyz;
	
	float3	vAvgPrevDir	= normalize(vPrevDir + vCurDir).xyz;
	float3	vAvgNextDir	= normalize(vCurDir + vNextDir).xyz;
	
	float3	vPrevOrtho	= normalize(cross(vAvgPrevDir, g_vCamPosition.xyz - In[0].vStart.xyz)) * g_fThickness;
	float3	vNextOrtho	= normalize(cross(vAvgNextDir, g_vCamPosition.xyz - In[1].vEnd.xyz)) * g_fThickness;
	
	Out[0].vPosition	= float4(In[0].vStart.xyz + vPrevOrtho, 1.f);
	Out[0].vPosition	= mul(Out[0].vPosition, mWVP);
	Out[0].vTexCoord	= float2(0.f, 0.f);
	Out[0].vColor		= In[0].vColor;
	Out[1].vPosition	= float4(In[0].vStart.xyz - vPrevOrtho, 1.f);
	Out[1].vPosition	= mul(Out[1].vPosition, mWVP);
	Out[1].vTexCoord	= float2(0.f, 1.f);
	Out[1].vColor		= In[0].vColor;
	
	Out[2].vPosition	= float4(In[1].vEnd.xyz + vNextOrtho, 1.f);
	Out[2].vPosition	= mul(Out[2].vPosition, mWVP);
	Out[2].vTexCoord	= float2(1.f, 1.f);
	Out[2].vColor		= In[1].vColor;
	Out[3].vPosition	= float4(In[1].vEnd.xyz - vNextOrtho, 1.f);
	Out[3].vPosition	= mul(Out[3].vPosition, mWVP);
	Out[3].vTexCoord	= float2(1.f, 0.f);
	Out[3].vColor		= In[1].vColor;

	Triangles.Append(Out[0]);
	Triangles.Append(Out[1]);
	Triangles.Append(Out[2]);
	Triangles.RestartStrip();

	Triangles.Append(Out[1]);
	Triangles.Append(Out[3]);
	Triangles.Append(Out[2]);
	Triangles.RestartStrip();
}

struct PS_IN
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor		: SV_TARGET0;
};

struct PS_OUT_POSTPROCESS
{
	float4 vPreProcess	: SV_TARGET0;
	float4 vMask		: SV_TARGET1;
};

PS_OUT_POSTPROCESS PS_MAIN(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;

    Out.vPreProcess		= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
    Out.vPreProcess.w	*= lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
    Out.vMask			= float4(0.f, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_LINE(PS_IN In)
{
    PS_OUT_POSTPROCESS Out;

    Out.vPreProcess		= g_vMtrlDiffuse;
    Out.vPreProcess.w	*= lerp(In.vColor.w, In.vColor.w - (1.f / g_iMaxInstance), In.vTexCoord.x);
    Out.vMask			= float4(0.f, 0.f, 0.f, 0.f);
	
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

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Line
	{
		VertexShader	= compile vs_5_0 VS_MAIN_LINE();
		GeometryShader	= compile gs_5_0 GS_MAIN_LINE();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_LINE();

		SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
}
