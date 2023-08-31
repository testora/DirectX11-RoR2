#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
private:
	explicit CModel(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CModel(const CModel&);
	virtual ~CModel() DEFAULT;

public:
	virtual HRESULT							Initialize(const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit);
	HRESULT									Render(shared_ptr<class CShader>, _uint iPassIndex);
	HRESULT									Render(_uint iMeshIndex, shared_ptr<class CShader>, _uint iPassIndex);

private:
#if ACTIVATE_TOOL
	HRESULT									Initialize_FromAssimp(const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit);
#endif
	HRESULT									Initialize_FromBinary(const wstring& wstrModelPath);

#if ACTIVATE_TOOL
	HRESULT									Ready_Bones(const aiNode*, _uint iParentBoneIndex);
	HRESULT									Ready_Animations(const aiScene*);
	HRESULT									Ready_Meshes(const aiScene*, _matrixf mPivot);
	HRESULT									Ready_Materials(const aiScene*, const wstring& wstrModelPath);
#endif

public:
	_uint									Get_BoneIndex(const _char* szBoneName);
#if ACTIVATE_TOOL
	const _uint								Get_NumBones() const				{ return m_iNumBones; }
	const _uint								Get_NumAnimations() const			{ return m_iNumAnimations; }
	const _uint								Get_NumMeshes() const				{ return m_iNumMeshes; }
	const _uint								Get_NumMaterials() const			{ return m_iNumMaterials; }

	shared_ptr<class CBone>					Get_Bone(_uint iIndex) const		{ return m_vecBones[iIndex]; }
	shared_ptr<class CAnimation>			Get_Animation(_uint iIndex) const	{ return m_vecAnimations[iIndex]; }
	shared_ptr<class CMesh>					Get_Mesh(_uint iIndex) const		{ return m_vecMeshes[iIndex]; }
	MATERIAL								Get_Material(_uint iIndex) const	{ return m_vecMaterials[iIndex]; }
#endif

public:
	void									Tick_Animation(_float fTimeDelta);
	void									Set_Animation(_uint iAnimationIndex, _float fInterpolationDuration, _bool bLoop = true);

	void									Add_ShaderBinding(_uint iMeshIndex, function<HRESULT()>);
	void									Iterate_Meshes(function<_bool(shared_ptr<class CMesh>)>);

private:
	HRESULT									Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, _uint iTextureIndex = 0);
	HRESULT									Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const _char* szConstantName, _uint iTextureIndex = 0);
	HRESULT									Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>);
	HRESULT									Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const _char* szConstantName);
	HRESULT									Bind_BoneMatrices(_uint iMeshIndex, shared_ptr<class CShader>, const _char* szConstantName);

private:
	MODEL									m_eType						= MODEL::MAX;

	_uint									m_iNumBones					= 0;
	vector<shared_ptr<class CBone>>			m_vecBones;

	_uint									m_iNumAnimations			= 0;
	vector<shared_ptr<class CAnimation>>	m_vecAnimations;

	_uint									m_iNumMeshes				= 0;
	vector<shared_ptr<class CMesh>>			m_vecMeshes;

	_uint									m_iNumMaterials				= 0;
	vector<MATERIAL>						m_vecMaterials;

	_uint									m_iCurrentAnimationIndex	= 0;
	_bool									m_bAnimLoop					= true;

	map<_uint, function<HRESULT()>>			m_mapMeshShaderBinding;

public:
	static shared_ptr<CModel>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit);
	virtual shared_ptr<CComponent>			Clone(any = any()) override;

#if ACTIVATE_TOOL
	HRESULT									Export(const wstring& wstrPath);
#endif
};

END
