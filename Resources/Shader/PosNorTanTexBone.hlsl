#include "ShaderCommon.hlsli"

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
	float4	vTangent		: TANGENT;
	float2	vTexCoord		: TEXCOORD0;
	float4	vWorldPos		: TEXCOORD1;
	float4	vProjPos		: TEXCOORD2;
};

struct VS_OUT_SHADOWDEPTH
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: TEXCOORD0;
	float4	vProjPos		: TEXCOORD1;
	float4	vDirection		: TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;
	
	float4x4	mWV		= mul(g_mWorld, g_mView);
	float4x4	mWVP	= mul(mWV, g_mProj);
	
	float4x4	mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
	float4 vPosition	= mul(float4(In.vPosition, 1.f), mBone);
	float4 vNormal		= mul(float4(In.vNormal, 0.f), mBone);
	float4 vTangent		= mul(float4(In.vTangent, 0.f), mBone);

	Out.vPosition		= mul(vPosition, mWVP);
	Out.vNormal			= normalize(mul(vNormal, g_mWorld));
	Out.vTangent		= normalize(mul(vTangent, g_mWorld));
	Out.vTexCoord		= In.vTexCoord;
	Out.vWorldPos		= mul(vPosition, g_mWorld);
	Out.vProjPos		= Out.vPosition;

	return Out;
}

VS_OUT_SHADOWDEPTH VS_MAIN_SHADOWDEPTH(VS_IN In)
{
    VS_OUT_SHADOWDEPTH	Out;
	
	float4x4	mWV		= mul(g_mWorld, g_mView);
	float4x4	mWVP	= mul(mWV, g_mProj);
	
	float4x4	mBone =
		mul(g_mBones[In.vBlendIndices.x], In.vBlendWeights.x) +
		mul(g_mBones[In.vBlendIndices.y], In.vBlendWeights.y) +
		mul(g_mBones[In.vBlendIndices.z], In.vBlendWeights.z) +
		mul(g_mBones[In.vBlendIndices.w], (1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z)));
	
	float4 vPosition	= mul(float4(In.vPosition, 1.f), mBone);
	float4 vNormal		= mul(float4(In.vNormal, 0.f), mBone);
	
	Out.vNormal			= normalize(mul(vNormal, g_mWorld));
	Out.vPosition		= mul(vPosition, mWVP);
	Out.vProjPos		= Out.vPosition;
	Out.vDirection		= g_mView[2];
	
    return Out;
}

struct PS_IN
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: NORMAL;
	float4	vTangent		: TANGENT;
	float2	vTexCoord		: TEXCOORD0;
	float4	vWorldPos		: TEXCOORD1;
	float4	vProjPos		: TEXCOORD2;
};

struct PS_IN_SHADOWDEPTH
{
	float4	vPosition		: SV_POSITION;
	float4	vNormal			: TEXCOORD0;
	float4	vProjPos		: TEXCOORD1;
	float4	vDirection		: TEXCOORD2;
};

struct PS_OUT
{
//	float4	vColor			: SV_TARGET0;
	float4	vDiffuse		: SV_TARGET0;
	float4	vAmbient		: SV_TARGET1;
	float4	vSpecular		: SV_TARGET2;
	float4	vEmissive		: SV_TARGET3;
	float4	vNormal			: SV_TARGET4;
	float4	vDepth			: SV_TARGET5;
};

struct PS_OUT_SHADOWDEPTH
{
	float4	vShadowDepth	: SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out;
	
	float3		vFinalColor;
	float4		vTexColor	=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
	// Normal
	float3		vNormal		=	In.vNormal.xyz;
	
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		float4		vUnityDXT5nm	= g_texNormal[0].Sample(LinearSampler, In.vTexCoord);
		float3x3	mTBN			= float3x3(In.vTangent.xyz, normalize(cross(In.vNormal.xyz, In.vTangent.xyz)), In.vNormal.xyz);
		vNormal						= UnpackNormalDXT5nm(vUnityDXT5nm, mTBN);
	}
	
	Out.vDiffuse	= vTexColor * g_vMtrlDiffuse;
	Out.vAmbient	= vTexColor * g_vMtrlAmbient;
	Out.vSpecular	= g_vMtrlSpecular;
	Out.vEmissive	= g_vMtrlEmissive;
	Out.vNormal		= float4(vNormal, 0.f) * 0.5f + 0.5f;
	Out.vDepth		= float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, 0.f);
	
//	// Specular
//	float3		vViewDir	=	normalize(g_vCamPosition - In.vWorldPos).xyz;
//	
//	for (int i = 0; i < g_iLightCount; ++i)
//	{
//		// Lighting
//		float3	vLightDir	=	float3(0.f, 0.f, 0.f);
//		switch (g_iLightType[i])
//		{
//		case POINT:
//			vLightDir		=	normalize(In.vWorldPos - g_vLightPosition[i]).xyz;
//			break;
//		case DIRECTIONAL:
//			vLightDir		=	normalize(g_vLightDirection[i]).xyz;
//			break;
//		}
//		
//		float	fDiffuse	=	saturate(dot(-vLightDir, vNormal) + DIFFUSE_OFFSET);
//		
//		// Specular
//		float3	vHalfDir	=	normalize(vViewDir - vLightDir);
//		float	fSpc		=	pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
//		
//		// Attenuation
//		float	fSmoothAtt	= 1.f;
//		if (POINT == g_iLightType[i])
//		{
//			float	fDist		=	distance(g_vLightPosition[i], In.vWorldPos);
//			float	fAtt		=	1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
//			float	fLightDist	=	1.f - saturate(fDist / g_fLightRange[i]);
//			fSmoothAtt			=	smoothstep(0.f, 1.f, fAtt * fLightDist);
//		}
//		
//		// Final
//		vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * g_vMtrlDiffuse.rgb * fDiffuse + g_vLightAmbient[i].rgb * g_vMtrlAmbient.rgb)
//							+	g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb * fSpc;
//		
//		vFinalColor			*=	fSmoothAtt;
//	}
//	
//	// Final
//	vFinalColor			+=	g_vMtrlEmissive.rgb;
//	
//	// Out
//	Out.vColor.rgb		=	vFinalColor;
//	Out.vColor.a		=	vTexColor.a;

	return Out;
}

PS_OUT_SHADOWDEPTH PS_MAIN_SHADOWDEPTH(PS_IN_SHADOWDEPTH In)
{
    PS_OUT_SHADOWDEPTH	Out;
	
//	if (0.f > dot(In.vNormal, In.vDirection))
//	{
//	    discard;
//	}
	
    Out.vShadowDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.f, 0.f);
	
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

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass WIREFRAME
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_WireFrame);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

    pass SHADOWDEPTH
    {
		VertexShader	= compile vs_5_0 VS_MAIN_SHADOWDEPTH();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN_SHADOWDEPTH();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
    }
}
