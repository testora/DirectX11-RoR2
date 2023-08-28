#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer
{
private:
	explicit CMesh(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CMesh() DEFAULT;

public:
	virtual HRESULT					Initialize(MODEL, const aiMesh*, shared_ptr<class CModel> pModel, _matrixf mPivot);

public:
	_uint							Get_MaterialIndex() const	{ return m_iMaterialIndex; }
	_float4x4*						Get_BoneMatrices(vector<shared_ptr<class CBone>>);
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	const _char*					Get_Name() const			{ return m_szName; }
#endif
#endif

	void							Set_Interpolation(vector<shared_ptr<class CBone>>, _float fDuration);

public:
	HRESULT							Ready_VertexBuffer_NonAnim(const aiMesh*);
	HRESULT							Ready_VertexBuffer_Anim(const aiMesh*, shared_ptr<class CModel> pModel);

private:
	_char							m_szName[MAX_PATH]		= "";

	_uint							m_iMaterialIndex		= 0;
	_float4x4						m_mPivot;

	_uint							m_iNumBones				= 0;
	vector<_uint>					m_vecBoneIndices;

	array<_float4x4, g_iMaxBones>	m_arrBones;
	vector<float4x4>				m_vecOffsets;

	array<_float4x4, g_iMaxBones>	m_arrInterpolationMatrices;
	_float							m_fInterpolationRatio	= 0.f;

public:
	static shared_ptr<CMesh>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, MODEL, const aiMesh*, shared_ptr<class CModel>, _matrixf mPivot);
	virtual shared_ptr<CComponent>	Clone(any = any()) override;
};

END
