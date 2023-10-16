#include "ShaderCommon.hlsli"

Texture2D	g_texMtrlDiffuseTarget;
Texture2D	g_texMtrlAmbientTarget;
Texture2D	g_texMtrlSpecularTarget;
Texture2D	g_texMtrlEmissiveTarget;
Texture2D	g_texNormalTarget;
Texture2D	g_texDepthTarget;
Texture2D	g_texShadeTarget;
Texture2D	g_texSpecularTarget;
Texture2D	g_texPreProcessTarget;
Texture2D	g_texMaskTarget;

float		g_fTime		= 0.f;

bool		g_bFog		= false;
float4		g_vFogColor	= float4(1.f, 1.f, 1.f, 1.f);
float		g_fFogStart	= 0.f;
float		g_fFogEnd	= 1.f;
float		g_fFogMax	= 1.f;
float		g_fFogPower	= 1.f;

struct VS_IN
{
	float3	vPosition	: POSITION;
	float2	vTexCoord	: TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
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
	float2	vTexCoord	: TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor		: SV_TARGET0;
};

struct PS_OUT_LIGHT
{
	float4	vShade		: SV_TARGET0;
	float4	vSpecular	: SV_TARGET1;
};

struct PS_OUT_POSTPROCESS
{
	float4	vPreProcess	: SV_TARGET0;
	float4	vMask		: SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT	Out;
	
	float4			vNormalTarget	= g_texNormalTarget.Sample(PointSampler, In.vTexCoord);
	float4			vNormal			= vNormalTarget * 2.f - 1.f;
	
	float4			vDepthTarget	= g_texDepthTarget.Sample(PointSampler, In.vTexCoord);
	float			fViewZ			= vDepthTarget.y * g_fCamFar;

	float			fShade			= saturate(dot(normalize(g_vLightDirection), -vNormal));
	fShade							= floor(fShade * CELL_SHADE_FREQUENCY) / CELL_SHADE_FREQUENCY;
	
	Out.vShade		= fShade;
	
	float4			vPosition;
	
	vPosition.x		= In.vTexCoord.x * +2.f - 1.f;
	vPosition.y		= In.vTexCoord.y * -2.f + 1.f;
	vPosition.z		= vDepthTarget.x;
	vPosition.w		= 1.f;
	
	vPosition		*= fViewZ;
	vPosition		= mul(vPosition, g_mProjInv);
	vPosition		= mul(vPosition, g_mViewInv);
	
	float4			vLook			= vPosition - g_vCamPosition;
	float4			vHalfWay		= normalize(normalize(g_vLightDirection) + normalize(-vLook));
	
	Out.vSpecular	= pow(abs(dot(vNormal, vHalfWay)), g_fMtrlShininess);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT	Out;
	
	float4			vNormalTarget	= g_texNormalTarget.Sample(PointSampler, In.vTexCoord);
	float4			vNormal			= vNormalTarget * 2.f - 1.f;
	
	float4			vDepthTarget	= g_texDepthTarget.Sample(PointSampler, In.vTexCoord);
	float			fViewZ			= vDepthTarget.y * g_fCamFar;

	float4			vPosition;
	
	vPosition.x		= In.vPosition.x * +2.f - 1.f;
	vPosition.y		= In.vPosition.y * -2.f + 1.f;
	vPosition.z		= vDepthTarget.x;
	vPosition.w		= 1.f;
	
	vPosition		*= fViewZ;
	vPosition		= mul(vPosition, g_mProjInv);
	vPosition		= mul(vPosition, g_mViewInv);
	
	float4			vLightDir		= vPosition - g_vLightPosition;
	float			fDistance		= length(vLightDir);
	
	float			fAtt			= max((g_fLightRange - fDistance) / g_fLightRange, 0.f);
	
	Out.vShade		= g_vLightDiffuse * saturate(max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) * fAtt
					+ (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
	
	float4			vLook			= vPosition - g_vCamPosition;
	float4			vHalfWay		= normalize(normalize(g_vLightDirection) + normalize(-vLook));
	
	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(dot(vNormal, vHalfWay), 1);
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_PREPROCESS(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	float4	vMtrlDiffuse	= g_texMtrlDiffuseTarget.Sample(LinearSampler, In.vTexCoord);
	if (vMtrlDiffuse.a == 0.f)
	{
		discard;
	}
	float4	vMtrlAmbient	= g_texMtrlAmbientTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vMtrlSpecular	= g_texMtrlSpecularTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vMtrlEmissive	= g_texMtrlEmissiveTarget.Sample(LinearSampler, In.vTexCoord);
	
	float4	vShade			= g_texShadeTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vSpecular		= g_texSpecularTarget.Sample(LinearSampler, In.vTexCoord);
	
	Out.vPreProcess			= vMtrlDiffuse	* g_vLightDiffuse	* vShade
							+ vMtrlAmbient	* g_vLightAmbient
							+ vMtrlSpecular	* g_vLightSpecular	* vSpecular
							+ vMtrlEmissive;
	
	Out.vMask				= float4(0.f, 0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT PS_MAIN_POSTPROCESS(PS_IN In)
{
	PS_OUT Out;
	
	float4	vMask	= g_texMaskTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vDepth	= g_texDepthTarget.Sample(LinearSampler, In.vTexCoord);
	
	if (0.f < vMask.a && vMask.a < 0.1f) 
	{
		float	fWaveLengthX	= vMask.x * 100.f;
		float	fWaveLengthY	= vMask.y * 100.f;
		float	fFrequency		= vMask.z * 100.f;
		
		float2	vDistortion		= float2(sin(In.vTexCoord.x + g_fTime * fFrequency * 0.01f) * fWaveLengthX, cos(In.vTexCoord.y + g_fTime * fFrequency * 0.01f) * fWaveLengthY);
		
		Out.vColor	= g_texPreProcessTarget.Sample(LinearSampler, In.vTexCoord + vDistortion);
	}
	else
	{
		Out.vColor	= g_texPreProcessTarget.Sample(LinearSampler, In.vTexCoord);
	}
	
	if (g_bFog)
	{
		float	fFogFactor	= saturate(vDepth.y);
		fFogFactor			= smoothstep(g_fFogStart, g_fFogEnd, fFogFactor);
		fFogFactor			= pow(fFogFactor, g_fFogPower);
		Out.vColor			= lerp(Out.vColor, g_vFogColor, min(fFogFactor, g_fFogMax));
	}

	return Out;
}

technique11 DefaultTechnique
{
	pass Directional
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_DIRECTIONAL();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Accumulate, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

	pass Point
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_POINT();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Accumulate, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }

	pass PreProcess
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_PREPROCESS();

		SetRasterizerState(RS_Default);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_IgnoreDepth, 0);
	}

	pass PostProcess
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_POSTPROCESS();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_IgnoreDepth, 0);
    }
}
