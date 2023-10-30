#include "ShaderCommon.hlsli"

Texture2D	g_texScopeTarget;

float		g_fCrosshairBoundsX;
float		g_fCrosshairBoundsY;

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

struct VS_OUT_WORLD
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
};

struct VS_OUT_ORTHOGRAPHIC
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float2	vLocalPos	: TEXCOORD1;
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

VS_OUT_WORLD VS_WORLD(VS_IN In)
{
	VS_OUT_WORLD Out;

	matrix	mWV, mWVP;

	mWV				= mul(g_mWorld, g_mView);
	mWVP			= mul(mWV, g_mProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= mul(float4(In.vPosition, 1.f), g_mWorld);

	return Out;
}

VS_OUT_WORLD VS_RAILGUNNER_PISTOLBULLET(VS_IN In)
{
	VS_OUT_WORLD Out;
	
	float4x4		mWorld			= g_mWorld;
	float			fDistance		= distance(g_vCamPosition, g_mWorld[3]);
	float			fMaxDist		= 50.f;
	
	if (fMaxDist < fDistance)
	{
		float		fRatio			= fDistance / fMaxDist;
		float4x4	fScaleFactor	= float4x4(
										float4(fRatio, 0.f, 0.f, 0.f),
										float4(0.f, fRatio, 0.f, 0.f),
										float4(0.f, 0.f, fRatio, 0.f),
										float4(0.f, 0.f, 0.f, 1.f));
		mWorld		= mul(fScaleFactor, mWorld);
	}
	  
	matrix	mWV, mWVP;

	mWV				= mul(mWorld, g_mView);
	mWVP			= mul(mWV, g_mProj);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), mWVP);
	Out.vTexCoord	= In.vTexCoord;
	Out.vWorldPos	= float4(In.vPosition, fDistance);

	return Out;
}

VS_OUT_ORTHOGRAPHIC VS_ORTHOGRAPHIC(VS_IN In)
{
	VS_OUT_ORTHOGRAPHIC	Out;
	
	Out.vPosition	= mul(float4(In.vPosition, 1.f), mul(g_mWorld, g_mOrthographic));
	Out.vTexCoord	= In.vTexCoord;
	Out.vLocalPos	= In.vPosition.xy;

	return Out;
}

struct PS_IN
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
};

struct PS_IN_WORLD
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float4	vWorldPos	: TEXCOORD1;
};

struct PS_IN_ORTHOGRAPHIC
{
	float4	vPosition	: SV_POSITION;
	float2	vTexCoord	: TEXCOORD0;
	float2	vLocalPos	: TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor		: SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT	Out;

	Out.vColor	=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	Out.vColor	*=	g_vMtrlDiffuse;
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_BOUNDS(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize					= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalTexture			= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vTextureSize			= float2(384.f, 384.f);
	
	float	fTexInterval			= 3.f;
	float	fTexBlendInterval		= 18.f;
	float	fTexOutlineThickness	= 12.f;
	float	fTexBoundHeight			= 94.f;
	
	float	fInGameBlendInterval	= 3.f;
	float	fInGameOutlineThickness	= 2.f;
	float	fInGameBoundHeight		= 16.f;
	
	float2	vTexCoord;
	
	if (vLocalTexture.x < fInGameBlendInterval)
	{
		vTexCoord.x = lerp(fTexInterval, fTexInterval + fTexBlendInterval, vLocalTexture.x / fInGameBlendInterval);
	}
	else if (vLocalTexture.x < fInGameBlendInterval + fInGameOutlineThickness)
	{
		vTexCoord.x = lerp(fTexInterval + fTexBlendInterval, fTexInterval + fTexBlendInterval + fTexOutlineThickness, (vLocalTexture.x - fInGameBlendInterval) / fInGameOutlineThickness);
	}
	else if (vLocalTexture.x < fInGameBlendInterval * 2.f + fInGameOutlineThickness)
	{
		vTexCoord.x = lerp(fTexInterval + fTexBlendInterval + fTexOutlineThickness, fTexInterval + fTexBlendInterval * 2.f + fTexOutlineThickness, (vLocalTexture.x - fInGameBlendInterval - fInGameOutlineThickness) / fInGameBlendInterval);
	}
	else if (vLocalTexture.x > vSize.x - fInGameBlendInterval)
	{
		vTexCoord.x = lerp(vTextureSize.x - fTexBlendInterval - fTexInterval, vTextureSize.x - fTexInterval, 1.f - (vSize.x - vLocalTexture.x) / fInGameBlendInterval);
	}
	else if (vLocalTexture.x > vSize.x - fInGameBlendInterval - fInGameOutlineThickness)
	{
		vTexCoord.x = lerp(vTextureSize.x - fTexBlendInterval - fTexOutlineThickness - fTexInterval, vTextureSize.x - fTexBlendInterval - fTexInterval, 1.f - (vSize.x - vLocalTexture.x - fInGameBlendInterval) / fInGameOutlineThickness);
	}
	else if (vLocalTexture.x > vSize.x - fInGameBlendInterval * 2.f - fInGameOutlineThickness)
	{
		vTexCoord.x = lerp(vTextureSize.x - fTexBlendInterval * 2.f - fTexOutlineThickness - fTexInterval, vTextureSize.x - fTexBlendInterval - fTexOutlineThickness - fTexInterval, 1.f - (vSize.x - vLocalTexture.x - fInGameBlendInterval - fInGameOutlineThickness) / fInGameBlendInterval);
	}
	else
	{
		vTexCoord.x = lerp(fTexInterval + fTexBlendInterval * 2.f + fTexOutlineThickness, vTextureSize.x - fTexBlendInterval * 2.f - fTexOutlineThickness - fTexInterval, (vLocalTexture.x - fInGameBlendInterval * 2.f - fInGameOutlineThickness) / (vSize.x - (fInGameBlendInterval * 2.f + fInGameOutlineThickness) * 2.f));
	}
	
	if (vLocalTexture.y < fInGameBlendInterval)
	{
		vTexCoord.y = lerp(fTexInterval, fTexInterval + fTexBlendInterval, vLocalTexture.y / fInGameBlendInterval);
	}
	else if (vLocalTexture.y > vSize.y - fInGameBlendInterval)
	{
		vTexCoord.y = lerp(vTextureSize.y - fTexBlendInterval - fTexInterval, vTextureSize.y - fTexInterval, 1.f - (vSize.y - vLocalTexture.y) / fInGameBlendInterval);
	}
	else if (vLocalTexture.y < fInGameBlendInterval + fInGameOutlineThickness)
	{
		vTexCoord.y = lerp(fTexInterval + fTexBlendInterval, fTexInterval + fTexBlendInterval + fTexOutlineThickness, (vLocalTexture.y - fInGameBlendInterval) / fInGameOutlineThickness);
	}
	else if (vLocalTexture.y > vSize.y - fInGameBlendInterval - fInGameOutlineThickness)
	{
		vTexCoord.y = lerp(vTextureSize.y - fTexBlendInterval - fTexOutlineThickness - fTexInterval, vTextureSize.y - fTexBlendInterval - fTexInterval, 1.f - (vSize.y - vLocalTexture.y - fInGameBlendInterval) / fInGameOutlineThickness);
	}
	else if (vLocalTexture.y < fInGameBlendInterval * 2.f + fInGameOutlineThickness)
	{
		vTexCoord.y = lerp(fTexInterval + fTexBlendInterval + fTexInterval + fTexOutlineThickness, fTexBlendInterval * 2.f + fTexOutlineThickness, (vLocalTexture.y - fInGameBlendInterval - fInGameOutlineThickness) / fInGameBlendInterval);
	}
	else if (vLocalTexture.y > vSize.y - fInGameBlendInterval * 2.f - fInGameOutlineThickness)
	{
		vTexCoord.y = lerp(vTextureSize.y - fTexBlendInterval * 2.f - fTexOutlineThickness - fTexInterval, vTextureSize.y - fTexBlendInterval - fTexOutlineThickness - fTexInterval, 1.f - (vSize.y - vLocalTexture.y - fInGameBlendInterval - fInGameOutlineThickness) / fInGameBlendInterval);
	}
	else if (vLocalTexture.y < fInGameBlendInterval + fInGameBoundHeight)
	{
		vTexCoord.y = lerp(fTexInterval + fTexBlendInterval * 2.f + fTexOutlineThickness, fTexInterval + fTexBlendInterval + fTexBoundHeight, (vLocalTexture.y - fInGameBlendInterval * 2.f - fInGameOutlineThickness) / (fInGameBoundHeight - fInGameOutlineThickness - fInGameBlendInterval));
	}
	else if (vLocalTexture.y > vSize.y - fInGameBlendInterval - fInGameBoundHeight)
	{
		vTexCoord.y = lerp(vTextureSize.y - fTexBlendInterval - fTexBoundHeight - fTexInterval, vTextureSize.y - fTexBlendInterval * 2.f - fTexOutlineThickness - fTexInterval, 1.f - (vSize.y - vLocalTexture.y - fInGameBlendInterval * 2.f - fInGameOutlineThickness) / (fInGameBoundHeight - fInGameOutlineThickness - fInGameBlendInterval));
	}
	else
	{
		vTexCoord.y = lerp(fTexInterval + fTexBlendInterval + fTexBoundHeight, vTextureSize.y - fTexBlendInterval - fTexBoundHeight - fTexInterval, (vLocalTexture.y - fInGameBlendInterval * 2.f - fInGameBoundHeight) / (vSize.y - (fInGameBlendInterval + fInGameBoundHeight) * 2.f));
	}
	
	Out.vColor	=	g_texDiffuse[0].Sample(LinearSampler, vTexCoord / vTextureSize);
	Out.vColor	*=	g_vMtrlDiffuse;
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_BRACKET(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize				= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalTexture		= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vTextureSize		= float2(256.f, 256.f);
	
	float	fTexInterval		= 10.f;
	float	fTexLineThickness	= 20.f;
	float	fTexLineWidth		= 50.f;
	float	fTexLineHeight		= 236.f;
	
	float	fInGameThickness	= 6.f;
	float	fInGameLineWidth	= 15.f;
	
	float2	vTexCoord;
	
	if (vLocalTexture.x < fInGameThickness)
	{
		vTexCoord.x = lerp(fTexInterval, fTexInterval + fTexLineThickness, vLocalTexture.x / fInGameThickness);
	}
	else if (vLocalTexture.x < fInGameLineWidth)
	{
		vTexCoord.x = lerp(fTexInterval + fTexLineThickness, fTexInterval + fTexLineWidth, (vLocalTexture.x - fInGameThickness) / (fInGameLineWidth - fInGameThickness));
	}
	else if (vLocalTexture.x > vSize.x - fInGameThickness)
	{
		vTexCoord.x = lerp(vTextureSize.x - (fTexInterval + fTexLineThickness), vTextureSize.x - fTexInterval, 1.f - (vSize.x - vLocalTexture.x) / fInGameThickness);
	}
	else if (vLocalTexture.x > vSize.x - fInGameLineWidth)
	{
		vTexCoord.x = lerp(vTextureSize.x - (fTexInterval + fTexLineWidth), vTextureSize.x - (fTexInterval + fTexLineThickness), 1.f - (vSize.x - vLocalTexture.x - fInGameThickness) / (fInGameLineWidth - fInGameThickness));
	}
	else
	{
		vTexCoord.x = lerp(fTexInterval + fTexLineWidth, vTextureSize.x - (fTexInterval + fTexLineWidth), (vLocalTexture.x - fInGameLineWidth) / (vSize.x - fInGameLineWidth * 2.f));
	}
	
	if (vLocalTexture.y < fInGameThickness)
	{
		vTexCoord.y = lerp(fTexInterval, fTexInterval + fTexLineThickness, vLocalTexture.y / fInGameThickness);
	}
	else if (vLocalTexture.y > vSize.y - fInGameThickness)
	{
		vTexCoord.y = lerp(vTextureSize.y - (fTexInterval + fTexLineThickness), vTextureSize.y - fTexInterval, 1.f - (vSize.y - vLocalTexture.y) / fInGameThickness);
	}
	else
	{
		vTexCoord.y = lerp(fTexInterval + fTexLineThickness, vTextureSize.y - (fTexInterval + fTexLineThickness), (vLocalTexture.y - fInGameThickness) / (vSize.y - fInGameThickness * 2.f));
	}
	
	Out.vColor	=	g_texDiffuse[0].Sample(LinearSampler, vTexCoord / vTextureSize);
	Out.vColor	*=	g_vMtrlDiffuse;
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_NIBS(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize				= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalTexture		= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vTextureSize		= float2(256.f, 256.f);
	
	float2	vTexInterval		= float2(63.f, 10.f);
	float2	vTexNibSize			= float2(36.f, 20.f);
	
	float2	vInGameNibSize		= float2(11.f, 6.f);
	
	float2	vTexCoord;
	
	if (vLocalTexture.x < vInGameNibSize.x)
	{
		vTexCoord.x = lerp(vTexInterval.x, vTexInterval.x + vTexNibSize.x, vLocalTexture.x / vInGameNibSize.x);
	}
	else if (vLocalTexture.x > vSize.x - vInGameNibSize.x)
	{
		vTexCoord.x = lerp(vTextureSize.x - (vTexInterval.x + vTexNibSize.x), vTextureSize.x - vTexInterval.x, 1.f - (vSize.x - vLocalTexture.x) / vInGameNibSize.x);
	}
	else
	{
		vTexCoord.x = lerp(vTexInterval.x + vTexNibSize.x, vTextureSize.x - (vTexInterval.x + vTexNibSize.x), (vLocalTexture.x - vInGameNibSize.x) / (vSize.x - vInGameNibSize.x * 2.f));
	}
	
	if (vLocalTexture.y < vInGameNibSize.y)
	{
		vTexCoord.y = lerp(vTexInterval.y, vTexInterval.y + vTexNibSize.y, vLocalTexture.y / vInGameNibSize.y);
	}
	else if (vLocalTexture.y > vSize.y - vInGameNibSize.y)
	{
		vTexCoord.y = lerp(vTextureSize.y - (vTexInterval.y + vTexNibSize.y), vTextureSize.y - vTexInterval.y, 1.f - (vSize.y - vLocalTexture.y) / vInGameNibSize.y);
	}
	else
	{
		vTexCoord.y = lerp(vTexInterval.y + vTexNibSize.y, vTextureSize.y - (vTexInterval.y + vTexNibSize.y), (vLocalTexture.y - vInGameNibSize.y) / (vSize.y - vInGameNibSize.y * 2.f));
	}
	
	Out.vColor	=	g_texDiffuse[1].Sample(LinearSampler, vTexCoord / vTextureSize);
	Out.vColor	*=	g_vMtrlDiffuse;
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_SCOPE(PS_IN_ORTHOGRAPHIC In)
{
	// Render Target
	// 0. Pure Camera Sight
	// 1. 
	// Crosshair	: Base-Black, Alpha-Over Black
	// Scope		: Base-Black, Alpha-Over Black
	// Outline		: Base-Black, Alpha-Over Yellow, TexCoord-x1.5(CenterBase)
	// Background	: Base-Black, Alpha-Over Yellow, TexCoord-x2.0(CenterBase)
	
	PS_OUT	Out;
	
	float2	vTexCoord = In.vTexCoord * 2.f - 1.f;
	
	if (In.vLocalPos.x < 0.f)
	{
		vTexCoord.x *= -1.f;
	}
	if (In.vLocalPos.y < 0.f)
	{
		vTexCoord.y *= -1.f;
	}
	
	Out.vColor				= g_texDiffuse[1].Sample(LinearSampler, vTexCoord);
	
	if (AvgGrayScale(Out.vColor.rgb) < 0.1f)
	{
		Out.vColor.a = 0.f;
	}
	
	float3	fYellowColor	= float3(1.f, 1.f, 0.f);
    float	fYellowFactor	= length(Out.vColor.rgb - fYellowColor);
	if (fYellowFactor < 0.1f)
	{
		Out.vColor			= g_texScopeTarget.Sample(TargetSampler, In.vTexCoord * 0.5f + 0.25f);
		Out.vColor.rgb		*= 0.25f;
		Out.vColor.a		= 1.f;
		
		return Out;
	}
	
	float3	fOutlineColor	= float3(0.4f, 0.4f, 0.f);
	float	fOutlineFactor	= length(Out.vColor.rgb - fOutlineColor);
	if (fOutlineFactor < 0.1f)
	{
		Out.vColor			= g_texScopeTarget.Sample(TargetSampler, In.vTexCoord * 0.75f + 0.125f);
		Out.vColor.rgb		*= 0.5f;
		Out.vColor.a		= 1.f;
		
		return Out;
	}
	
	float3	fCrossColor		= float3(0.f, 1.f, 0.f);
	float	fCrossFactor	= length(Out.vColor.rgb - fCrossColor);
	if (fCrossFactor < 0.7f)
	{
		return Out;
	}
	
	Out.vColor.rgb			= float3(0.f, 0.f, 0.f);
	
	return Out;
}

PS_OUT PS_RAILGUNNER_PISTOLBULLET(PS_IN_WORLD In)
{
	PS_OUT	Out;
	
	float	fDistance			= In.vWorldPos.w;
	float	fTextureCycleFactor	= 0.1f;
	float	fColorCycleFactor	= 0.1f;
	float2	vLocalPosition		= In.vWorldPos.xy;
	float4	vMaskColor1			= float4(0.99f, 0.95f, 0.70f, 1.00f);
	float4	vMaskColor2			= float4(1.00f, 0.40f, 0.30f, 1.00f);
	
	float	fCycle				= fmod(fDistance * fColorCycleFactor, 1.f);
	float	fSubCycle			= fmod(fCycle, 0.5f);
	bool	bCycle				= fCycle < 0.5f;
	bool	bInside				= abs(vLocalPosition.x) < fSubCycle && abs(vLocalPosition.y) < fSubCycle;
	
	float	fOffsetFactor		= fmod(fDistance * fTextureCycleFactor, 1.f);
	float2	vCenter				= float2(0.5f, 0.5f);
	float2	vOffset				= (In.vTexCoord - vCenter) * fOffsetFactor;
	float2	vTexCoord			= In.vTexCoord - vOffset;
	
	Out.vColor		=	g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
	if (bInside)
	{
		Out.vColor	*=	bCycle ? vMaskColor1 : vMaskColor2;
	}
	else
	{
	Out.vColor	*=	bCycle ? vMaskColor2 : vMaskColor1;
	}
	
	return Out;
}

PS_OUT PS_BROTHER_HEALTHBAR_BACKGROUND(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize				= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalTexture		= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vBackgroundSize		= float2(960.f, 24.f);
	float	fAlphaInterval		= 100.f;
	
	float2	vTexCoord = In.vTexCoord * 2.f - 1.f;
	
	Out.vColor = float4(0.5f, 0.5f, 0.5f, 0.5f);
	
	if (vLocalTexture.x < fAlphaInterval)
	{
		Out.vColor.a = lerp(0.f, Out.vColor.a, vLocalTexture.x / fAlphaInterval);
	}
	else if (vLocalTexture.x > vBackgroundSize.x - fAlphaInterval)
	{
		Out.vColor.a = lerp(0.f, Out.vColor.a, (vBackgroundSize.x - vLocalTexture.x) / fAlphaInterval);
	}
	
	return Out;
}

PS_OUT PS_BROTHER_HEALTHBAR_HEALTH(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	Out.vColor = float4(0.6f, 0.f, 0.f, 1.f);
	
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

	pass Orthographic
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass RailGunner_Crosshair_Bounds
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_BOUNDS();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass RailGunner_Crosshair_Bracket
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_BRACKET();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass RailGunner_Crosshair_Nibs
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_NIBS();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass RailGunner_Crosshair_Scope
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_SCOPE();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass RailGunner_PistolBullet
	{
		VertexShader	= compile vs_5_0 VS_RAILGUNNER_PISTOLBULLET();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_PISTOLBULLET();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Brother_HealthBar_Background
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_BROTHER_HEALTHBAR_BACKGROUND();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}

	pass Brother_HealthBar_Health
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_BROTHER_HEALTHBAR_HEALTH();

		SetRasterizerState(RS_Default);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
	}
}
