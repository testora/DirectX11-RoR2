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
};

struct VS_OUT
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
};

struct VS_OUT_QUAD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vWorldPos0	: TEXCOORD1;
	float4 vWorldPos1	: TEXCOORD2;
	float4 vWorldPos2	: TEXCOORD3;
	float4 vWorldPos3	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
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

VS_OUT VS_MAIN_BILLBOARD(VS_IN In)
{
	VS_OUT Out;
	
	float4x4			mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	float4				vPosition		= mul(float4(In.vPosition, 1.f), mTransformation);

	Out.vPosition		= mul(vPosition, g_mWorld);
	Out.vSize			= In.vSize;
	Out.vColor			= In.vColor;
	
	return Out;
}

VS_OUT VS_MAIN_WORLD(VS_IN In)
{
	VS_OUT Out;
	
	float4x4			mTransformation	= float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	float4				vPosition		= mul(float4(In.vPosition, 1.f), mTransformation);

	Out.vPosition		= mul(vPosition, mTransformation);
	Out.vSize			= In.vSize;
	Out.vColor			= In.vColor;

	return Out;
}

VS_OUT_QUAD VS_MAIN_QUAD(VS_IN In)
{
    VS_OUT_QUAD Out;
	
	float4x4			mWV, mWVP;

	mWV					= mul(g_mWorld, g_mView);
	mWVP				= mul(mWV, g_mProj);

	Out.vPosition		= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vSize			= In.vSize;
	
	Out.vWorldPos0		= In.vRight;
	Out.vWorldPos1		= In.vUp;
	Out.vWorldPos2		= In.vLook;
	Out.vWorldPos3		= In.vTranslation;
	Out.vColor			= In.vColor;

	return Out;
}

struct GS_IN
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
};

struct GS_IN_QUAD
{
	float4 vPosition	: POSITION;
	float2 vSize		: TEXCOORD0;
	
	float4 vWorldPos0	: TEXCOORD1;
	float4 vWorldPos1	: TEXCOORD2;
	float4 vWorldPos2	: TEXCOORD3;
	float4 vWorldPos3	: TEXCOORD4;
	float4 vColor		: TEXCOORD5;
};

struct GS_OUT
{
	float4 vPosition	: SV_POSITION;
	float2 vTexCoord	: TEXCOORD0;
	float4 vColor		: TEXCOORD1;
};

[maxvertexcount(6)]
void GS_MAIN_RECT(point GS_IN In[1], inout TriangleStream<GS_OUT> Triangles)
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
void GS_MAIN_RECT_BILLBOARD(point GS_IN In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];
	
	float4 vLook		= g_vCamPosition - In[0].vPosition;
	float3 vRight		= normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vSize.x * 0.5f;
	float3 vUp			= normalize(cross(vLook.xyz, vRight)) * In[0].vSize.y * 0.5f;

	matrix matVP		= mul(g_mView, g_mProj);

	Out[0].vPosition	= float4(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition	= mul(Out[0].vPosition, matVP);
	Out[0].vTexCoord	= float2(0.f, 0.f);
	Out[0].vColor		= In[0].vColor;

	Out[1].vPosition	= float4(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition	= mul(Out[1].vPosition, matVP);
	Out[1].vTexCoord	= float2(1.f, 0.f);
	Out[1].vColor		= In[0].vColor;

	Out[2].vPosition	= float4(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition	= mul(Out[2].vPosition, matVP);
	Out[2].vTexCoord	= float2(1.f, 1.f);
	Out[2].vColor		= In[0].vColor;

	Out[3].vPosition	= float4(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition	= mul(Out[3].vPosition, matVP);
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
void GS_MAIN_QUAD(point GS_IN_QUAD In[1], inout TriangleStream<GS_OUT> Triangles)
{
	GS_OUT Out[4];

	matrix matVP		= mul(g_mView, g_mProj);

	Out[0].vPosition	= float4(In[0].vWorldPos0);
	Out[0].vPosition	= mul(Out[0].vPosition, matVP);
	Out[0].vTexCoord	= float2(0.f, 0.f);
	Out[0].vColor		= In[0].vColor;

	Out[1].vPosition	= float4(In[0].vWorldPos1);
	Out[1].vPosition	= mul(Out[1].vPosition, matVP);
	Out[1].vTexCoord	= float2(1.f, 0.f);
	Out[1].vColor		= In[0].vColor;

	Out[2].vPosition	= float4(In[0].vWorldPos2);
	Out[2].vPosition	= mul(Out[2].vPosition, matVP);
	Out[2].vTexCoord	= float2(1.f, 1.f);
	Out[2].vColor		= In[0].vColor;

	Out[3].vPosition	= float4(In[0].vWorldPos3);
	Out[3].vPosition	= mul(Out[3].vPosition, matVP);
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

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
	return Out;
}

PS_OUT PS_MAIN_NONEUV(PS_IN In)
{
	PS_OUT Out;

    Out.vColor = g_vMtrlDiffuse * In.vColor;
	
	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Billboard
	{
		VertexShader	= compile vs_5_0 VS_MAIN_BILLBOARD();
		GeometryShader	= compile gs_5_0 GS_MAIN_RECT_BILLBOARD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Trail
	{
		VertexShader	= compile vs_5_0 VS_MAIN_QUAD();
		GeometryShader	= compile gs_5_0 GS_MAIN_QUAD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

        SetRasterizerState(RS_NoneCull);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Trail_NoneUV
	{
		VertexShader	= compile vs_5_0 VS_MAIN_QUAD();
		GeometryShader	= compile gs_5_0 GS_MAIN_QUAD();
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_NONEUV();

        SetRasterizerState(RS_NoneCull);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
}
