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
#if ACTIVATE_TOOL
	HRESULT							Initialize_FromAssimp(MODEL, const aiMesh*, shared_ptr<class CModel>);
#endif
	HRESULT							Initialize_FromBinary(MODEL, std::ifstream&);

#if ACTIVATE_TOOL
private:
	HRESULT							Ready_VertexBuffer_Anim(const aiMesh*, shared_ptr<class CModel> pModel);
	HRESULT							Ready_VertexBuffer_NonAnim(const aiMesh*);
#endif
	HRESULT							Ready_VertexBuffer_Anim(std::ifstream&);
	HRESULT							Ready_VertexBuffer_NonAnim(std::ifstream&);

public:
	_uint							Get_NumBones() const					{ return m_iNumBones; }

	const _char*					Get_Name() const						{ return m_szName; }
	const _uint						Get_MaterialIndex() const				{ return m_iMaterialIndex; }
	const _float4x4*				Get_BoneMatrices(vector<shared_ptr<class CBone>>::iterator itBegin, _matrixf mPivot);

	void							Set_InterpolationMatrix(vector<shared_ptr<class CBone>>::iterator itBegin, _float fDuration);

private:
	_char							m_szName[MAX_PATH]		= "";

	_uint							m_iMaterialIndex		= 0;

	_uint							m_iNumBones				= 0;
	vector<_uint>					m_vecBoneIndices;
	vector<float4x4>				m_vecBoneOffsets;

	vector<_float4x4>				m_vecBones;
	vector<_float4x4>				m_vecInterpolationMatrices;
	_float							m_fInterpolationRatio	= 0.f;

#if ACTIVATE_TOOL
	unique_ptr<VTXMESH[]>			m_pVertices_NonAnim;
	unique_ptr<VTXMESHANIM[]>		m_pVertices_Anim;
#endif

public:
#if ACTIVATE_TOOL
	static shared_ptr<CMesh>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, MODEL, const aiMesh*, shared_ptr<class CModel>);
#endif
	static shared_ptr<CMesh>		Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, MODEL, std::ifstream&);
	static shared_ptr<CMesh>		Read(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrPath);
	virtual shared_ptr<CComponent>	Clone(any = g_aNull) override;

#if ACTIVATE_TOOL
	void							Export(std::ofstream&, MODEL);
	HRESULT							Export(const wstring& wstrPath);
#endif
};

END
