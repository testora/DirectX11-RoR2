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

	return Out;
}

PS_OUT PS_RAILGUNNER_CROSSHAIR(PS_IN_ORTHOGRAPHIC In)
{
	PS_OUT	Out;
	
	float2	vSize				= float2(g_mWorld._11, g_mWorld._22);
	float2	vLocalPos			= (In.vLocalPos + 0.5f) * vSize;
	
	float2	vTextureSize		= float2(384.f, 384.f);
	
	float	fTexInterval		= 21.f;
	float	fTexOutlineWidth	= 12.f;
	float	fTexBoundHeight		= 94.f;
	
	float	fInGameOutlineWidth	= 4.f;
	float	fInGameBoundHeight	= 32.f;
	
	float2	vTexCoord;
	
	if (vLocalPos.x < fInGameOutlineWidth)
	{
		vTexCoord.x = lerp(fTexInterval, fTexInterval + fTexOutlineWidth, vLocalPos.x / fInGameOutlineWidth) / vTextureSize.x;
	}
	else if (vLocalPos.x > vSize.x - fInGameOutlineWidth)
	{
		vTexCoord.x = lerp(vTextureSize.x - (fTexInterval + fTexOutlineWidth), vTextureSize.x - fTexInterval, 1.f - (vSize.x - vLocalPos.x) / fInGameOutlineWidth) / vTextureSize.x;
	}
	else
	{
		vTexCoord.x = lerp(fTexInterval + fTexOutlineWidth, vTextureSize.x - (fTexInterval + fTexOutlineWidth), (vLocalPos.x - fInGameOutlineWidth) / (vSize.x - fInGameOutlineWidth * 2.f)) / vTextureSize.x;
	}
	
	if (vLocalPos.y < fInGameOutlineWidth)
	{
		vTexCoord.y = lerp(fTexInterval, fTexInterval + fTexOutlineWidth, vLocalPos.y / fInGameOutlineWidth) / vTextureSize.y;
	}
	else if (vLocalPos.y < fInGameBoundHeight)
	{
		vTexCoord.y = lerp(fTexInterval + fTexOutlineWidth, fTexInterval + fTexBoundHeight, (vLocalPos.y - fInGameOutlineWidth) / (fInGameBoundHeight - fInGameOutlineWidth)) / vTextureSize.y;
	}
	else if (vLocalPos.y > vSize.y - fInGameOutlineWidth)
	{
		vTexCoord.y = lerp(vTextureSize.y - (fTexInterval + fTexOutlineWidth), vTextureSize.y - fTexInterval, 1.f - (vSize.y - vLocalPos.y) / fInGameOutlineWidth) / vTextureSize.y;
	}
	else if (vLocalPos.y > vSize.y - fInGameBoundHeight)
	{
		vTexCoord.y = lerp(vTextureSize.y - (fTexInterval + fTexBoundHeight), vTextureSize.y - (fTexInterval + fTexOutlineWidth), 1.f - (vSize.y - vLocalPos.y - fInGameOutlineWidth) / (fInGameBoundHeight - fInGameOutlineWidth)) / vTextureSize.y;
	}
	else
	{
		vTexCoord.y = lerp(fTexInterval + fTexBoundHeight, vTextureSize.y - (fTexInterval + fTexBoundHeight), (vLocalPos.y - fInGameBoundHeight) / (vSize.y - fInGameBoundHeight * 2.f)) / vTextureSize.y;
	}
	
	Out.vColor	= g_texDiffuse[0].Sample(LinearSampler, vTexCoord);
	
	if (Out.vColor.a < 1.f)
	{
		discard;
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
	}

	pass Orthographic
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_MAIN();
	}

	pass RailGunner_Crosshair
	{
		VertexShader	= compile vs_5_0 VS_ORTHOGRAPHIC();
		GeometryShader	= NULL;
		HullShader		= NULL;
		DomainShader	= NULL;
		PixelShader		= compile ps_5_0 PS_RAILGUNNER_CROSSHAIR();
	}
}