#include "ShaderCommon.hlsli"

float4x4			g_mQuadWorld;
float4x4			g_mQuadView;
float4x4			g_mQuadProj;

Texture2D			g_texShadowDepthTarget[MAX_LIGHT];
Texture2D			g_texMtrlDiffuseTarget;
Texture2D			g_texMtrlAmbientTarget;
Texture2D			g_texMtrlSpecularTarget;
Texture2D			g_texMtrlEmissiveTarget;
Texture2D			g_texNormalTarget;
Texture2D			g_texDepthTarget;
Texture2D			g_texLightDiffuseTarget;
Texture2D			g_texLightAmbientTarget;
Texture2D			g_texLightSpecularTarget;
Texture2D			g_texPreProcessTarget;
Texture2D			g_texMaskTarget;

int					g_iShadowCount		= 0;
float4x4			g_mShadowViews[MAX_LIGHT];
float4x4			g_mShadowProjs[MAX_LIGHT];

float				g_fTime				= 0.f;

bool				g_bFog				= false;
float4				g_vFogColor			= float4(1.f, 1.f, 1.f, 1.f);
float				g_fFogStart			= 0.f;
float				g_fFogEnd			= 1.f;
float				g_fFogAmbient		= 0.f;
float				g_fFogMax			= 1.f;
float				g_fFogPower			= 1.f;

float				fBias				= -4.f;
static const float	fShadowShade		= 0.5f;
static const float	fMinShadowAvg		= 0.0f;
static const int2	vGridSize			= int2(12, 12);
static const float2	vTexel				= float2(1.f / 1920.f, 1.f / 1080.f) * 0.2f;

float4 ShadowDepthTargetLiteral(int iIndex, float2 vUV);

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

	mWV				= mul(g_mQuadWorld, g_mQuadView);
	mWVP			= mul(mWV, g_mQuadProj);

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
	float4	vDiffuse	: SV_TARGET0;
	float4	vAmbient	: SV_TARGET1;
	float4	vSpecular	: SV_TARGET2;
};

struct PS_OUT_POSTPROCESS
{
	float4	vPreProcess	: SV_TARGET0;
	float4	vMask		: SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT	Out;
	
	float4			vNormalTarget	= g_texNormalTarget.Sample(TargetSampler, In.vTexCoord);
	float4			vNormal			= vNormalTarget * 2.f - 1.f;
	
	float4			vDepthTarget	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	float			fViewZ			= vDepthTarget.y * g_fCamFar;

	float			fShade			= saturate(dot(normalize(g_vLightDirection), -vNormal));
	fShade							= floor(fShade * CELL_SHADE_FREQUENCY) / CELL_SHADE_FREQUENCY;

//	
//	Out.vShade		= fShade;
//	Out.vShade		= g_vLightDiffuse * fShade;
	Out.vDiffuse	= g_vLightDiffuse * fShade;
//	Out.vAmbient	= g_vLightAmbient;
	Out.vAmbient	= g_vLightAmbient * (fShade * 0.5f + 0.5f);
	
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

//	
//	Out.vSpecular	= pow(abs(dot(vNormal, vHalfWay)), g_fMtrlShininess);
	Out.vSpecular	= g_vLightSpecular * pow(saturate(dot(vNormal, vHalfWay)), g_fMtrlShininess);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT	Out;
	
	float4			vNormalTarget	= g_texNormalTarget.Sample(TargetSampler, In.vTexCoord);
	float4			vNormal			= vNormalTarget * 2.f - 1.f;
	
	float4			vDepthTarget	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	float			fViewZ			= vDepthTarget.y * g_fCamFar;
	
	float4			vPosition;
	
	vPosition.x		= In.vTexCoord.x * +2.f - 1.f;
	vPosition.y		= In.vTexCoord.y * -2.f + 1.f;
	vPosition.z		= vDepthTarget.x;
	vPosition.w		= 1.f;
	
	vPosition		*= fViewZ;
	vPosition		= mul(vPosition, g_mProjInv);
	vPosition		= mul(vPosition, g_mViewInv);
	
//	
//	float4			vLightDir		= vPosition - g_vLightPosition;
//	float			fDistance		= length(vLightDir);
//	
//	float			fAtt			= max((g_fLightRange - fDistance) / g_fLightRange, 0.f);
//	
//	Out.vShade		= g_vLightDiffuse * saturate(max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) * fAtt
//					+ (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
//	
//	float4			vLook			= vPosition - g_vCamPosition;
//	float4			vHalfWay		= normalize(normalize(g_vLightDirection) + normalize(-vLook));
//	
//	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(dot(vNormal, vHalfWay), 1);

	float4			vLightDirection	= vPosition - g_vLightPosition;
	float			fLightDistance	= length(vLightDirection);
	vLightDirection	= normalize(vLightDirection);
	
	float			fShade			= saturate(dot(normalize(vLightDirection), -vNormal));
	fShade			= floor(fShade * CELL_SHADE_FREQUENCY) / CELL_SHADE_FREQUENCY;
	
	float			fAtt			= saturate(fLightDistance / g_fLightRange);
	
	Out.vDiffuse	= g_vLightDiffuse * fShade * fAtt;
	Out.vAmbient	= float4(0.f, 0.f, 0.f, 0.f);
	
	float4			vLook			= vPosition - g_vCamPosition;
	float4			vHalfWay		= normalize(vLightDirection + normalize(-vLook));
	
	Out.vSpecular	= g_vLightSpecular * pow(saturate(dot(vNormal, vHalfWay)), g_fMtrlShininess) * fAtt;
	
	return Out;
}

PS_OUT_POSTPROCESS PS_MAIN_PREPROCESS(PS_IN In)
{
	PS_OUT_POSTPROCESS Out;
	
	Out.vMask				= float4(0.f, 0.f, 0.f, 0.f);
	
//	ShadowDepth
	float4	vDepthTarget	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);
	if (vDepthTarget.w == 1.f)
	{
		discard;
	}
	float	fViewZ			= vDepthTarget.y * g_fCamFar;
	
//	Pre Process	
	float4	vMtrlDiffuse	= g_texMtrlDiffuseTarget.Sample(LinearSampler, In.vTexCoord);
	if (vMtrlDiffuse.a == 0.f)
	{
		discard;
	}
	float4	vMtrlAmbient	= g_texMtrlAmbientTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vMtrlSpecular	= g_texMtrlSpecularTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vMtrlEmissive	= g_texMtrlEmissiveTarget.Sample(LinearSampler, In.vTexCoord);
	
	float4	vDiffuse		= g_texLightDiffuseTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vAmbient		= g_texLightAmbientTarget.Sample(LinearSampler, In.vTexCoord);
	float4	vSpecular		= g_texLightSpecularTarget.Sample(LinearSampler, In.vTexCoord);
	
	Out.vPreProcess			= vMtrlDiffuse	* vDiffuse
							+ vMtrlAmbient	* vAmbient
							+ vMtrlSpecular	* vSpecular,
							+ vMtrlEmissive;
	
//	ShadowDepth
	float4	vPosition;
	vPosition.x		= In.vTexCoord.x * +2.f - 1.f;
	vPosition.y		= In.vTexCoord.y * -2.f + 1.f;
	vPosition.z		= vDepthTarget.x;
	vPosition.w		= 1.f;
	
	vPosition		*= fViewZ;
	vPosition		= mul(vPosition, g_mProjInv);
	vPosition		= mul(vPosition, g_mViewInv);
	
    for (int i = 0; i < g_iShadowCount; ++i)
	{
		float4	vShadowView			= mul(vPosition, g_mShadowViews[i]);
		float4	vShadowProj			= mul(vShadowView, g_mShadowProjs[i]);
		
		float2	vShadowUV			= vShadowProj.xy / vShadowProj.w;
		vShadowUV.x					= vShadowUV.x * +0.5f + 0.5f;
		vShadowUV.y					= vShadowUV.y * -0.5f + 0.5f;
		
		if (saturate(vShadowUV.x) != vShadowUV.x
		||	saturate(vShadowUV.y) != vShadowUV.y)
		{
			continue;
		}
		
	//	float4 vShadowDepthTarget = g_texShadowDepthTarget[i].SampleGrad(TargetSampler, vShadowUV, ddx(vShadowUV.x), ddy(vShadowUV.y));
    //	float4 vShadowDepthTarget = float4(0.f, 0.f, 0.f, 0.f);
		
		float fShadowWeight = 0.f;
		
        for (int y = -vGridSize.y * 0.5f; y <= vGridSize.y * 0.5f; ++y)
		{
            for (int x = -vGridSize.x * 0.5f; x <= vGridSize.x * 0.5f; ++x)
			{
			//	float4 vShadowDepthTarget = g_texShadowDepthTarget[i].SampleGrad(TargetSampler, vShadowUV + float2(x, y) * vTexel, ddx(vShadowUV.x), ddy(vShadowUV.y));
				float4 vShadowDepthTarget = ShadowDepthTargetLiteral(i, vShadowUV + float2(x, y) * vTexel);
				if (!vShadowDepthTarget.z && !vDepthTarget.z)
				{
					continue;
				}
				if (vShadowView.z + fBias > vShadowDepthTarget.y * g_fCamFar)
				{
					fShadowWeight += 1.f;
				}
			}
		}
		
		float fShadowAvg = fShadowWeight / (vGridSize.x * vGridSize.y);
		if (fShadowAvg < fMinShadowAvg)
		{
			continue;
		}

		Out.vPreProcess *= (1.f - fShadowAvg * fShadowShade);
		
	//	return Out;
		
//		float4	vShadowDepthTarget	= g_texShadowDepthTarget.Sample(TargetSampler, vShadowUV);
//		
//	//	if (vShadowDepthTarget.z == vDepthTarget.z)
//	//	{
//	//		return Out;
//	//	}
//				
//	//	if (vShadowDepthTarget.w == 1.f)
//	//	{
//	//		return Out;
//	//	}
//		
//		if (vShadowView.z + a > vShadowDepthTarget.y * g_fCamFar)
//		{
//			Out.vPreProcess *= 0.5f;
//		}
	}
	
	return Out;
}

PS_OUT PS_MAIN_POSTPROCESS(PS_IN In)
{
	PS_OUT Out;
	
	float4	vMask	= g_texMaskTarget.Sample(TargetSampler, In.vTexCoord);
	float4	vDepth	= g_texDepthTarget.Sample(TargetSampler, In.vTexCoord);

	if (0.f < vMask.a && vMask.a < 0.1f) 
	{
		float	fWaveLengthX	= vMask.x * 100.f;
		float	fWaveLengthY	= vMask.y * 100.f;
		float	fFrequency		= vMask.z * 100.f;
		
		float2	vDistortion		= float2(sin(In.vTexCoord.x + g_fTime * fFrequency * 0.01f) * fWaveLengthX, cos(In.vTexCoord.y + g_fTime * fFrequency * 0.01f) * fWaveLengthY);
		
		Out.vColor	= g_texPreProcessTarget.Sample(TargetSampler, In.vTexCoord + vDistortion);
	}
	else
	{
		Out.vColor	= g_texPreProcessTarget.Sample(TargetSampler, In.vTexCoord);
	}
	
	if (g_bFog)
	{
		float	fFogFactor	= saturate(vDepth.y);
		fFogFactor			= smoothstep(g_fFogStart, g_fFogEnd, clamp(fFogFactor, g_fFogStart, g_fFogEnd));
		fFogFactor			= pow(fFogFactor, g_fFogPower);
		Out.vColor			= lerp(Out.vColor, g_vFogColor, saturate(min(fFogFactor, g_fFogMax) + g_fFogAmbient));
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

float4 ShadowDepthTargetLiteral(int iIndex, float2 vUV)
{
	switch (iIndex)
	{
		case 0:
			return g_texShadowDepthTarget[0].SampleLevel(TargetSampler, vUV, 0);
		case 1:
            return g_texShadowDepthTarget[1].SampleLevel(TargetSampler, vUV, 0);
		case 2:
            return g_texShadowDepthTarget[2].SampleLevel(TargetSampler, vUV, 0);
		case 3:
            return g_texShadowDepthTarget[3].SampleLevel(TargetSampler, vUV, 0);
		case 4:
            return g_texShadowDepthTarget[4].SampleLevel(TargetSampler, vUV, 0);
		case 5:
            return g_texShadowDepthTarget[5].SampleLevel(TargetSampler, vUV, 0);
		case 6:
            return g_texShadowDepthTarget[6].SampleLevel(TargetSampler, vUV, 0);
		case 7:
            return g_texShadowDepthTarget[7].SampleLevel(TargetSampler, vUV, 0);
		case 8:
            return g_texShadowDepthTarget[8].SampleLevel(TargetSampler, vUV, 0);
		case 9:
            return g_texShadowDepthTarget[9].SampleLevel(TargetSampler, vUV, 0);
		case 10:
            return g_texShadowDepthTarget[10].SampleLevel(TargetSampler, vUV, 0);
		case 11:
            return g_texShadowDepthTarget[11].SampleLevel(TargetSampler, vUV, 0);
		case 12:
            return g_texShadowDepthTarget[12].SampleLevel(TargetSampler, vUV, 0);
		case 13:
            return g_texShadowDepthTarget[13].SampleLevel(TargetSampler, vUV, 0);
		case 14:
            return g_texShadowDepthTarget[14].SampleLevel(TargetSampler, vUV, 0);
		case 15:
            return g_texShadowDepthTarget[15].SampleLevel(TargetSampler, vUV, 0);
		case 16:
            return g_texShadowDepthTarget[16].SampleLevel(TargetSampler, vUV, 0);
		case 17:
            return g_texShadowDepthTarget[17].SampleLevel(TargetSampler, vUV, 0);
		case 18:
            return g_texShadowDepthTarget[18].SampleLevel(TargetSampler, vUV, 0);
		case 19:
            return g_texShadowDepthTarget[19].SampleLevel(TargetSampler, vUV, 0);
		case 20:
            return g_texShadowDepthTarget[20].SampleLevel(TargetSampler, vUV, 0);
		case 21:
            return g_texShadowDepthTarget[21].SampleLevel(TargetSampler, vUV, 0);
		case 22:
            return g_texShadowDepthTarget[22].SampleLevel(TargetSampler, vUV, 0);
		case 23:
            return g_texShadowDepthTarget[23].SampleLevel(TargetSampler, vUV, 0);
		case 24:
            return g_texShadowDepthTarget[24].SampleLevel(TargetSampler, vUV, 0);
		case 25:
            return g_texShadowDepthTarget[25].SampleLevel(TargetSampler, vUV, 0);
		case 26:
            return g_texShadowDepthTarget[26].SampleLevel(TargetSampler, vUV, 0);
		case 27:
            return g_texShadowDepthTarget[27].SampleLevel(TargetSampler, vUV, 0);
		case 28:
            return g_texShadowDepthTarget[28].SampleLevel(TargetSampler, vUV, 0);
		case 29:
            return g_texShadowDepthTarget[29].SampleLevel(TargetSampler, vUV, 0);
		case 30:
            return g_texShadowDepthTarget[30].SampleLevel(TargetSampler, vUV, 0);
		case 31:
            return g_texShadowDepthTarget[31].SampleLevel(TargetSampler, vUV, 0);
    }
	return float4(0.f, 0.f, 0.f, 0.f);
}
