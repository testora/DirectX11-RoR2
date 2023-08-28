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
	_uint							Get_MaterialIndex() const		{ return m_iMaterialIndex; }
	_float4x4*						Get_BoneMatrices(vector<shared_ptr<class CBone>>);
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	pair<const VTXMESH*, _uint>		Get_Vertices_NonAnim() const	{ return make_pair(m_pVertices_NonAnim.get(), m_iNumVertices); }
	pair<const VTXMESHANIM*, _uint>	Get_Vertices_Anim() const		{ return make_pair(m_pVertices_Anim.get(), m_iNumVertices); }
	pair<const _uint*, _uint>		Get_BoneIndices() const			{ return make_pair(m_vecBoneIndices.data(), m_iNumBones); }
	pair<const float4x4*, _uint>	Get_BoneOffsets() const			{ return make_pair(m_vecBoneOffsets.data(), m_iNumBones); }

	const _char*					Get_Name() const				{ return m_szName; }
	_float4x4						Get_Pivot() const				{ return m_mPivot; }
	_uint							Get_NumBones() const			{ return m_iNumBones; }

#endif
#endif

	void							Set_Interpolation(vector<shared_ptr<class CBone>>, _float fDuration);

public:
	HRESULT							Ready_VertexBuffer_NonAnim(const aiMesh*);
	HRESULT							Ready_VertexBuffer_Anim(const aiMesh*, shared_ptr<class CModel> pModel);

private:
	_char							m_szName[MAX_PATH]		= "";

	_float4x4						m_mPivot;
	_uint							m_iMaterialIndex		= 0;

	_uint							m_iNumBones				= 0;
	vector<_uint>					m_vecBoneIndices;
	vector<float4x4>				m_vecBoneOffsets;

	array<_float4x4, g_iMaxBones>	m_arrBones;
	array<_float4x4, g_iMaxBones>	m_arrInterpolationMatrices;
	_float							m_fInterpolationRatio	= 0.f;

#ifdef _DEBUG
#if ACTIVATE_IMGUI
	unique_ptr<VTXMESH[]>			m_pVertices_NonAnim;
	unique_ptr<VTXMESHANIM[]>		m_pVertices_Anim;
#endif
#endif

public:
	static shared_ptr<CMesh>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, MODEL, const aiMesh*, shared_ptr<class CModel>, _matrixf mPivot);
	virtual shared_ptr<CComponent>	Clone(any = any()) override;
};

END
