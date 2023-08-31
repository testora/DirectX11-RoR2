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
	
	vector vPosition	= mul(vector(In.vPosition, 1.f), mBone);
	vector vNormal		= mul(vector(normalize(In.vNormal), 0.f), mBone);
	vector vTangent		= mul(vector(normalize(In.vTangent), 0.f), mBone);

	Out.vPosition		= mul(vPosition, mWVP);
	Out.vNormal			= mul(vNormal, g_mWorld);
	Out.vTangent		= mul(vTangent, g_mWorld);
	Out.vTexCoord		= In.vTexCoord;
	Out.vWorldPos		= mul(vPosition, g_mWorld);

	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float4	vNormal		: NORMAL;
	float4	vTangent	: TANGENT;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
};

struct PS_OUT
{
	float4 vColor		: SV_TARGET0;
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
		float3x3	vTBN		=	float3x3(In.vTangent.xyz, cross(In.vNormal.xyz, In.vTangent.xyz), In.vNormal.xyz);
		float3		vNormalMap	=	g_texNormal[0].Sample(LinearSampler, In.vTexCoord).xyz * 2.f - 1.f;
		vNormal					=	mul(vNormalMap, vTBN);
	}
	
	// Specular
	float3		vViewDir	=	normalize(g_vCamPosition - In.vWorldPos).xyz;
	
	for (int i = 0; i < g_iLightCount; ++i)
	{
		// Lighting
		float3	vLightDir	=	float3(0.f, 0.f, 0.f);
		switch (g_iLightType[i])
		{
		case POINT:
			vLightDir		=	normalize(In.vWorldPos - g_vLightDirection[i]).xyz;
			break;
		case DIRECTIONAL:
			vLightDir		=	normalize(g_vLightDirection[i]).xyz;
			break;
		}
		
		float	fDiffuse	=	saturate(dot(-vLightDir, vNormal));
		
		// Specular
		float3	vHalfDir	=	normalize(vViewDir - vLightDir);
		float	fSpc		=	pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
		
		// Attenuation
		float	fSmoothAtt	= 1.f;
		if (POINT == g_iLightType[i])
		{
			float	fDist		=	distance(g_vLightPosition[i], In.vWorldPos);
			float	fAtt		=	1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
			float	fLightDist	=	1.f - saturate(fDist / g_fLightRange[i]);
			fSmoothAtt			=	smoothstep(0.f, 1.f, fAtt * fLightDist);
		}
		
		// Final
		vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * fDiffuse + g_vLightAmbient[i].rgb)
							+	fSpc * g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb;
		
		vFinalColor			*=	fSmoothAtt;
	}
	
	// Final
	vFinalColor			+=	g_vMtrlEmissive.rgb;
	
	// Out
	Out.vColor.rgb		=	vFinalColor;
	Out.vColor.a		=	vTexColor.a;

	return Out;
}

PS_OUT PS_TRIPLANER_MIX(PS_IN In)
{
	PS_OUT	Out;
	
	float3	vFinalColor;
	
	// TriPlaner
	float3	vBlendFactor	=	BlendFactor(In.vNormal.xyz);

	float4	vTexColor		=	TriPlanar_Mix_Diffuse(g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord * g_fTilingNormal[0]), In.vNormal.xyz, In.vWorldPos.xyz, vBlendFactor);
	
	// Normal
	float3	vNormal			=	In.vNormal.xyz;
	
	if (g_iShaderFlag & STATUS_TEXNORMAL)
	{
		vNormal				=	TriPlanar_Mix_Normal(In.vNormal.xyz, In.vTangent.xyz, In.vWorldPos.xyz, vBlendFactor);
	}
	
	// Specular
	float3	vViewDir		=	normalize(g_vCamPosition - In.vWorldPos).xyz;
	
	for (int i = 0; i < g_iLightCount; ++i)
	{
		// Lighting
		float3	vLightDir	=	float3(0.f, 0.f, 0.f);
		switch (g_iLightType[i])
		{
		case POINT:
			vLightDir		=	normalize(In.vWorldPos - g_vLightDirection[i]).xyz;
			break;
		case DIRECTIONAL:
			vLightDir		=	normalize(g_vLightDirection[i]).xyz;
			break;
		}
		
		float	fDiffuse	=	saturate(dot(-vLightDir, vNormal));
		
		// Specular
		float3	vHalfDir	=	normalize(vViewDir - vLightDir);
		float	fSpc		=	pow(max(dot(vViewDir, vHalfDir), 0.f), g_fMtrlShininess);
		
		// Attenuation
		float	fSmoothAtt	= 1.f;
		if (POINT == g_iLightType[i])
		{
			float	fDist		=	distance(g_vLightPosition[i], In.vWorldPos);
			float	fAtt		=	1.f / (g_fLightAttenuation0[i] + g_fLightAttenuation1[i] * fDist + g_fLightAttenuation2[i] * fDist * fDist);
			float	fLightDist	=	1.f - saturate(fDist / g_fLightRange[i]);
			fSmoothAtt			=	smoothstep(0.f, 1.f, fAtt * fLightDist);
		}
		
		// Final
		vFinalColor			+=	vTexColor.rgb * (g_vLightDiffuse[i].rgb * fDiffuse + g_vLightAmbient[i].rgb)
							+	fSpc * g_vLightSpecular[i].rgb * g_vMtrlSpecular.rgb;
		
		vFinalColor			*=	fSmoothAtt;
	}
	
	// Final
	vFinalColor			+=	g_vMtrlEmissive.rgb;
	
	// Out
	Out.vColor.rgb		=	vFinalColor;
	Out.vColor.a		=	vTexColor.a;

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

	pass MESH_TRIPLANER_MIX
	{
		VertexShader	= compile vs_5_0 VS_MAIN();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_TRIPLANER_MIX();
	}
}