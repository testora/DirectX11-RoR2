#pragma once

namespace Engine
{
#pragma region Window

	typedef struct tagGraphicDesc
	{
		enum WINMODE	{ FULLSCREEN, WINDOW, MAX };
		WINMODE			eWinMode;
		HWND			hWnd;
		_uint			iWinCX, iWinCY;

	} GRAPHICDESC;
	
#pragma endregion
#pragma region Vertex

	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexCoord;

		const static unsigned int				iNumElement = 2;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexCoord;

		const static unsigned int				iNumElement = 3;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTEX;

#pragma endregion
}
