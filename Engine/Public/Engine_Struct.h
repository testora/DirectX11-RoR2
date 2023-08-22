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
#pragma region Shader

	typedef struct tagLightDesc
	{
		enum LIGHTTYPE	{ DIRECTIONAL, POINT, MAX };
		LIGHTTYPE		eLightType;

		XMFLOAT3		vPosition;
		XMFLOAT3		vDirection;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vAmbient;

		float			fRange;
		float			fAttenuation0;
		float			fAttenuation1;
		float			fAttenuation2;
	} LIGHTDESC;

	typedef struct tagMaterialDesc
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vEmissive;
		float			fShininess;
	} MATERIALDESC;

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

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoordBones
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT2		vTexCoord;
		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;

		const static unsigned int				iNumElement = 6;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEXBONE, VTXMESHANIM;

#pragma endregion
#pragma region Model
	
	typedef struct tagModelMaterial
	{
		shared_ptr<class CTexture>	pMaterial[AI_TEXTURE_TYPE_MAX];
	} MODELMATERIAL;

#pragma endregion
#pragma region Animation

	typedef struct tagKeyFrame
	{
		XMFLOAT4	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT4	vTranslation;
		float		fTime;
	} KEYFRAME;

#pragma endregion
#pragma region Character

	typedef struct tagCharacterDesc
	{
		float		fForwardSpeed;
		float		fLeftSpeed;
		float		fBackwardSpeed;
		float		fRightSpeed;
		float		fJumpPower;

		XMFLOAT3	vMaxSpeed;
		XMFLOAT3	vResist;
	} CHARACTERDESC;

#pragma endregion
#pragma region Etc

	typedef struct tagXMFLOAT3Hash
	{
		size_t operator()(const DirectX::XMFLOAT3& k) const
		{
			return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
		}
	} XMFLOAT3HASH, FLOAT3HASH;

	typedef struct tagXMFLOAT3Equal
	{
		bool operator()(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs) const
		{
			return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
		}
	} XMFLOAT3EQUAL, FLOAT3EQUAL;

#pragma endregion
}
