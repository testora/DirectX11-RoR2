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

		_float			fRange;
		_float			fAttenuation0;
		_float			fAttenuation1;
		_float			fAttenuation2;
	} LIGHTDESC;

	typedef struct tagMaterialDesc
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
		XMFLOAT4		vEmissive;
		_float			fShininess;

		tagMaterialDesc() :
			vDiffuse	(1.f, 1.f, 1.f, 1.f),
			vAmbient	(1.f, 1.f, 1.f, 1.f),
			vSpecular	(1.f, 1.f, 1.f, 1.f),
			vEmissive	(0.f, 0.f, 0.f, 1.f),
			fShininess	(32.f)
		{}
	} MATERIALDESC;

#pragma endregion
#pragma region Vertex

	typedef struct ENGINE_DLL tagVertexPosition
	{
		XMFLOAT3		vPosition;

		const static _uint						iNumElement = 1;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOS;

	typedef struct ENGINE_DLL tagVertexPositionTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexCoord;

		const static _uint						iNumElement = 2;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexCoord;

		const static _uint						iNumElement = 3;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTEX;

	typedef struct ENGINE_DLL tagVertexPositionNormalTangentTexcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTangent;
		XMFLOAT2		vTexCoord;

		const static _uint						iNumElement = 4;
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

		const static _uint						iNumElement = 6;
		const static D3D11_INPUT_ELEMENT_DESC	tElements[iNumElement];
	} VTXPOSNORTANTEXBONE, VTXMESHANIM;

#pragma endregion
#pragma region Model
	
	typedef struct tagMaterial
	{
		shared_ptr<class CTexture>	pTexture[AI_TEXTURE_TYPE_MAX];
	} MATERIAL;

#pragma endregion
#pragma region Animation

	typedef struct tagKeyFrame
	{
		XMFLOAT4	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT4	vTranslation;
		_float		fTime;
	} KEYFRAME;

#pragma endregion
#pragma region Navigation

	typedef struct tagNavigationDesc
	{
		_uint		iInitialIndex;
		XMFLOAT4X4	mWorld;
	} NAVIGATIONDESC;

#pragma endregion
#pragma region Collider

	typedef struct tagColliderDesc
	{
		COLLIDER	eType;

		XMFLOAT3	vPosition;
		XMFLOAT3	vSize;
		XMFLOAT3	vRotation;
		_float 		fRadius;
	} COLLIDERDESC;

#pragma endregion
#pragma region Entity

	typedef struct tagEntityDesc
	{
		_float		fMaxHP;
		_float		fHP;

		_float		fForwardSpeed;
		_float		fLeftSpeed;
		_float		fBackwardSpeed;
		_float		fRightSpeed;
		_float		fSpritPower;
		_float		fJumpPower;

		XMFLOAT3	vMaxSpeed;
		XMFLOAT3	vResist;
	} ENTITYDESC;

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
