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

	typedef struct tagWndProcDesc
	{
		HWND			hWnd;
		UINT			message;
		WPARAM			wParam;
		LPARAM			lParam;
	} WNDPROCDESC;
	
#pragma endregion
#pragma region Light

	typedef struct tagLightDesc
	{
		enum LIGHTTYPE	{ DIRECTIONAL, POINT, MAX };
		XMFLOAT4	vDifuse;
		XMFLOAT4	vSpecular;
		XMFLOAT4	vAmbient;

		XMFLOAT3	vPosition;
		XMFLOAT3	vDirection;

		float		fRange;
		float		fAttenuation0;
		float		fAttenuation1;
		float		fAttenuation2;
	} LIGHTDESC;

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

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT2		vTexCoord;

		const static unsigned int				iNumElement = 4;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEX, VTXMESH;

#pragma endregion
#pragma region Model
	
	typedef struct tagModelMaterial
	{
		shared_ptr<class CTexture>	pMaterial[AI_TEXTURE_TYPE_MAX];
	} MODELMATERIAL;

#pragma endregion
}
