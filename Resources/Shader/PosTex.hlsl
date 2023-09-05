#include "ShaderCommon.hlsli"

float g_fCrosshairBoundsX;
float g_fCrosshairBoundsY;

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

	Out.vColor	= g_texDiffuse[0].Sample(LinearSampler, In.vTexCoord);
	
    if (Out.vColor.a < 0.1f)
    {
        discard; // Temp
    }
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_BOUNDS(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize					= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalPos				= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vTextureSize			= float2(384.f, 384.f);
	
	float	fTexInterval			= 21.f;
	float	fTexOutlineThickness	= 12.f;
	float	fTexBoundHeight			= 94.f;
	
    float	fInGameInterval			= 3.5f;
	float	fInGameOutlineThickness	= 2.f;
	float	fInGameBoundHeight		= 16.f;
	
	float2	vTexCoord;
	
	if (vLocalPos.x < fInGameInterval)
    {
		vTexCoord.x = lerp(0, fTexInterval, vLocalPos.x / fInGameInterval) / vTextureSize.x;
    }
	else if (vLocalPos.x < fInGameInterval + fInGameOutlineThickness)
    {
		vTexCoord.x = lerp(fTexInterval, fTexInterval + fTexOutlineThickness, (vLocalPos.x - fInGameInterval) / fInGameOutlineThickness) / vTextureSize.x;
    }
	else if (vLocalPos.x > vSize.x - fInGameInterval)
    {
        vTexCoord.x = lerp(vTextureSize.x - fTexInterval, vTextureSize.x, 1.f - (vSize.x - vLocalPos.x) / fInGameInterval) / vTextureSize.x;
    }
    else if (vLocalPos.x > vSize.x - fInGameInterval - fInGameOutlineThickness)
    {
        vTexCoord.x = lerp(vTextureSize.x - (fTexInterval + fTexOutlineThickness), vTextureSize.x - fTexInterval, 1.f - (vSize.x - vLocalPos.x - fInGameInterval) / fInGameOutlineThickness) / vTextureSize.x;
    }
    else
    {
        vTexCoord.x = lerp(fTexInterval + fTexOutlineThickness, vTextureSize.x - (fTexInterval + fTexOutlineThickness), (vLocalPos.x - fInGameInterval - fInGameOutlineThickness) / (vSize.x - fInGameInterval * 2.f - fInGameOutlineThickness * 2.f)) / vTextureSize.x;
    }
	
	if (vLocalPos.y < fInGameInterval)
    {
		vTexCoord.y = lerp(0, fTexInterval, vLocalPos.y / fInGameInterval) / vTextureSize.y;
    }
	else if(vLocalPos.y < fInGameInterval + fInGameOutlineThickness)
    {
		vTexCoord.y = lerp(fTexInterval, fTexInterval + fTexOutlineThickness, (vLocalPos.y - fInGameInterval) / fInGameOutlineThickness) / vTextureSize.y;
    }
	else if (vLocalPos.y < fInGameInterval + fInGameBoundHeight)
    {
        vTexCoord.y = lerp(fTexInterval + fTexOutlineThickness, fTexInterval + fTexBoundHeight, (vLocalPos.y - fInGameInterval - fInGameOutlineThickness) / (fInGameBoundHeight - fInGameOutlineThickness)) / vTextureSize.y;
    }
    else if (vLocalPos.y > vSize.y - fInGameInterval)
    {
        vTexCoord.y = lerp(vTextureSize.y - fTexInterval, vTextureSize.y, 1.f - (vSize.y - vLocalPos.y) / fInGameInterval) / vTextureSize.y;
    }
    else if (vLocalPos.y > vSize.y - fInGameInterval - fInGameOutlineThickness)
    {
        vTexCoord.y = lerp(vTextureSize.y - (fTexInterval + fTexOutlineThickness), vTextureSize.y - fTexInterval, 1.f - (vSize.y - vLocalPos.y - fInGameInterval) / fInGameOutlineThickness) / vTextureSize.y;
    }
    else if (vLocalPos.y > vSize.y - fInGameInterval - fInGameBoundHeight)
    {
        vTexCoord.y = lerp(vTextureSize.y - (fTexInterval + fTexBoundHeight), vTextureSize.y - (fTexInterval + fTexOutlineThickness), 1.f - (vSize.y - vLocalPos.y - fInGameInterval - fInGameOutlineThickness) / (fInGameBoundHeight - fInGameOutlineThickness)) / vTextureSize.y;
    }
	else
    {
		vTexCoord.y = lerp(fTexInterval + fTexBoundHeight, vTextureSize.y - (fTexInterval + fTexBoundHeight), (vLocalPos.y - fInGameInterval - fInGameBoundHeight) / (vSize.y - fInGameInterval * 2.f - fInGameBoundHeight * 2.f)) / vTextureSize.y;
    }
	
	Out.vColor	= g_texDiffuse[0].Sample(LinearSampler, vTexCoord);
	
	if (Out.vColor.a < 1.f)
	{
		discard;	// Temp
	}
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_BRACKET(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize				= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalPos			= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vTextureSize		= float2(256.f, 256.f);
	
	float	fTexInterval		= 10.f;
	float	fTexLineThickness	= 20.f;
	float	fTexLineWidth		= 50.f;
	float	fTexLineHeight		= 236.f;
	
	float	fInGameThickness	= 6.f;
	float	fInGameLineWidth	= 15.f;
	
	float2	vTexCoord;
	
	if (vLocalPos.x < fInGameThickness)
	{
		vTexCoord.x = lerp(fTexInterval, fTexInterval + fTexLineThickness, vLocalPos.x / fInGameThickness) / vTextureSize.x;
	}
	else if (vLocalPos.x < fInGameLineWidth)
	{
		vTexCoord.x = lerp(fTexInterval + fTexLineThickness, fTexInterval + fTexLineWidth, (vLocalPos.x - fInGameThickness) / (fInGameLineWidth - fInGameThickness)) / vTextureSize.x;
	}
	else if (vLocalPos.x > vSize.x - fInGameThickness)
	{
		vTexCoord.x = lerp(vTextureSize.x - (fTexInterval + fTexLineThickness), vTextureSize.x - fTexInterval, 1.f - (vSize.x - vLocalPos.x) / fInGameThickness) / vTextureSize.x;
	}
	else if (vLocalPos.x > vSize.x - fInGameLineWidth)
	{
        vTexCoord.x = lerp(vTextureSize.x - (fTexInterval + fTexLineWidth), vTextureSize.x - (fTexInterval + fTexLineThickness), 1.f - (vSize.x - vLocalPos.x - fInGameThickness) / (fInGameLineWidth - fInGameThickness)) / vTextureSize.x;
    }
	else
	{
		vTexCoord.x = lerp(fTexInterval + fTexLineWidth, vTextureSize.x - (fTexInterval + fTexLineWidth), (vLocalPos.x - fInGameLineWidth) / (vSize.x - fInGameLineWidth * 2.f)) / vTextureSize.x;
	}
	
	if (vLocalPos.y < fInGameThickness)
    {
		vTexCoord.y = lerp(fTexInterval, fTexInterval + fTexLineThickness, vLocalPos.y / fInGameThickness) / vTextureSize.y;
    }
	else if (vLocalPos.y > vSize.y - fInGameThickness)
    {
		vTexCoord.y = lerp(vTextureSize.y - (fTexInterval + fTexLineThickness), vTextureSize.y - fTexInterval, 1.f - (vSize.y - vLocalPos.y) / fInGameThickness) / vTextureSize.y;
    }
	else
    {
		vTexCoord.y = lerp(fTexInterval + fTexLineThickness, vTextureSize.y - (fTexInterval + fTexLineThickness), (vLocalPos.y - fInGameThickness) / (vSize.y - fInGameThickness * 2.f)) / vTextureSize.y;
    }
	
	Out.vColor = g_texDiffuse[0].Sample(LinearSampler, vTexCoord);
	
	if (Out.vColor.a < 1.f)
	{
		discard;	// Temp
	}
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_NIBS(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize				= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalPos			= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vTextureSize		= float2(256.f, 256.f);
	
	float2	vTexInterval		= float2(63.f, 10.f);
	float2	vTexNibSize			= float2(36.f, 20.f);
	
	float2	vInGameNibSize		= float2(11.f, 6.f);
	
	float2	vTexCoord;
	
    if (vLocalPos.x < vInGameNibSize.x)
	{
        vTexCoord.x = lerp(vTexInterval.x, vTexInterval.x + vTexNibSize.x, vLocalPos.x / vInGameNibSize.x) / vTextureSize.x;
    }
    else if (vLocalPos.x > vSize.x - vInGameNibSize.x)
	{
        vTexCoord.x = lerp(vTextureSize.x - (vTexInterval.x + vTexNibSize.x), vTextureSize.x - vTexInterval.x, 1.f - (vSize.x - vLocalPos.x) / vInGameNibSize.x) / vTextureSize.x;
    }
	else
	{
        vTexCoord.x = lerp(vTexInterval.x + vTexNibSize.x, vTextureSize.x - (vTexInterval.x + vTexNibSize.x), (vLocalPos.x - vInGameNibSize.x) / (vSize.x - vInGameNibSize.x * 2.f)) / vTextureSize.x;
    }
	
    if (vLocalPos.y < vInGameNibSize.y)
    {
        vTexCoord.y = lerp(vTexInterval.y, vTexInterval.y + vTexNibSize.y, vLocalPos.y / vInGameNibSize.y) / vTextureSize.y;
    }
    else if (vLocalPos.y > vSize.y - vInGameNibSize.y)
    {
        vTexCoord.y = lerp(vTextureSize.y - (vTexInterval.y + vTexNibSize.y), vTextureSize.y - vTexInterval.y, 1.f - (vSize.y - vLocalPos.y) / vInGameNibSize.y) / vTextureSize.y;
    }
	else
    {
        vTexCoord.y = lerp(vTexInterval.y + vTexNibSize.y, vTextureSize.y - (vTexInterval.y + vTexNibSize.y), (vLocalPos.y - vInGameNibSize.y) / (vSize.y - vInGameNibSize.y * 2.f)) / vTextureSize.y;
    }
	
	Out.vColor = g_texDiffuse[1].Sample(LinearSampler, vTexCoord);
	
	if (Out.vColor.a < 1.f)
	{
		discard;	// Temp
	}
	
	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR_SCOPE(PS_IN_ORTHOGRAPHIC In)
{
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
	
    Out.vColor = g_texDiffuse[1].Sample(LinearSampler, vTexCoord);
	
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
	}

	pass Orthographic
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}

	pass RailGunner_Crosshair_Bounds
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_BOUNDS();
	}

	pass RailGunner_Crosshair_Bracket
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_BRACKET();
	}

	pass RailGunner_Crosshair_Nibs
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_NIBS();
	}

    pass RailGunner_Crosshair_Scope
    {
        VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
        GeometryShader	= NULL;
        HullShader		= NULL;
        DomainShader	= NULL;
        PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR_SCOPE();
    }
}