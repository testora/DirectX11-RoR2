#define MAX_TEXTURE						8
#define	MAX_LIGHT						32
#define MAX_BONE						256
#define PI								3.141592654f

#define CELL_SHADE_FREQUENCY			4.f

#define DIFFUSE_OFFSET					0.f
#define TRIPLANAR_SLOT					1

#define DIRECTIONAL						0
#define POINT							1

#define STATUS_TEXDIFFUSE				0x0001
#define STATUS_TEXNORMAL				0x0002
#define STATUS_TRIPLANER_POSITIVE_X		0x0004
#define STATUS_TRIPLANER_POSITIVE_Y		0x0008
#define STATUS_TRIPLANER_POSITIVE_Z		0x0010
#define STATUS_TRIPLANER_NEGATIVE_X		0x0020
#define STATUS_TRIPLANER_NEGATIVE_Y		0x0040
#define STATUS_TRIPLANER_NEGATIVE_Z		0x0080
#define STATUS_TRIPLANER_SHARE_X        0x0100
#define STATUS_TRIPLANER_SHARE_Y        0x0200
#define STATUS_TRIPLANER_SHARE_Z        0x0400
#define STATUS_TRIPLANER_SHARE_X_Z      0x0800
#define STATUS_TRIPLANER_SYNC_XZ		0x1000

// Sampler

sampler LinearSampler = sampler_state
{
	Filter		            = MIN_MAG_MIP_LINEAR;
	AddressU	            = wrap;
	AddressV	            = wrap;
};

sampler PointSampler = sampler_state
{
	Filter		            = MIN_MAG_MIP_POINT;
	AddressU	            = wrap;
	AddressV	            = wrap;
};

// RasterizerState

RasterizerState RS_Default
{
	FillMode                = solid;
	CullMode                = back;
	FrontCounterClockwise   = false;
};

RasterizerState RS_InverseCull
{
	FillMode                = solid;
	CullMode                = front;
	FrontCounterClockwise   = false;
};

RasterizerState RS_NoneCull
{
	FillMode                = solid;
	CullMode                = none;
	FrontCounterClockwise   = false;
};

RasterizerState RS_WireFrame
{
	FillMode				= wireframe;
	CullMode				= back;
	FrontCounterClockwise	= false;
};

// BlendState

BlendState BS_Default
{
	BlendEnable[0]          = false;
};

BlendState BS_Accumulate
{
	BlendEnable[0]			= true;
	BlendEnable[1]			= true;

	SrcBlend				= One;
	DestBlend				= One;
	BlendOp					= Add;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0]          = true;
	SrcBlend                = Src_Alpha;
	DestBlend               = Inv_Src_Alpha;
	BlendOp                 = Add;
};

// DepthStencilState

DepthStencilState DSS_Default
{
	DepthEnable             = true;
	DepthWriteMask          = all;
	DepthFunc               = less_equal;

	StencilEnable			= false;
	FrontFaceStencilFunc	= less;
	FrontFaceStencilPass	= replace;
};

DepthStencilState DSS_IgnoreDepth
{
	DepthEnable             = false;
	DepthWriteMask          = zero;
	DepthFunc               = less_equal;

	StencilEnable			= false;
	FrontFaceStencilFunc	= less;
	FrontFaceStencilPass	= replace;
};
