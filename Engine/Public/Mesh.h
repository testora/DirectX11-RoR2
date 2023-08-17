#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer, public std::enable_shared_from_this<CMesh>
{
private:
	explicit CMesh(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CMesh() DEFAULT;

public:
	virtual HRESULT					Initialize(CModel::TYPE, const aiMesh*, shared_ptr<CModel> pModel, _matrixf mPivot);
	virtual HRESULT					Initialize(any pAIMesh) override;

public:
	_uint							Get_MaterialIndex() const	{ return m_iMaterialIndex; }
	_float4x4*						Get_BoneMatrices(vector<shared_ptr<class CBone>>);

public:
	HRESULT							Ready_VertexBuffer_NonAnim(const aiMesh*);
	HRESULT							Ready_VertexBuffer_Anim(const aiMesh*, shared_ptr<CModel> pModel);

private:
	char							m_szName[MAX_PATH]	= "";

	_uint							m_iMaterialIndex	= 0;
	_float4x4						m_mPivot;

	_uint							m_iNumBones			= 0;
	vector<_uint>					m_vecBoneIndices;

	vector<float4x4>				m_vecOffsets;
	array<_float4x4, g_iMaxBones>	m_arrBones;

public:
	static shared_ptr<CMesh>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, CModel::TYPE, const aiMesh*, shared_ptr<CModel>, _matrixf mPivot);
	virtual shared_ptr<CComponent>	Clone(any = any()) override;
};

END
