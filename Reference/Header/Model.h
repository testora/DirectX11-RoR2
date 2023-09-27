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
	HRESULT														Initialize_Prototype(const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit);
	HRESULT														Initialize(any mapMeshDesc = g_aNull);
	HRESULT														Render(shared_ptr<class CShader>, _uint iPassIndex);
	HRESULT														Render(_uint iMeshIndex, shared_ptr<class CShader>, _uint iPassIndex);

private:
#if ACTIVATE_TOOL
	HRESULT														Initialize_FromAssimp(const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit);
#endif
	HRESULT														Initialize_FromBinary(const wstring& wstrModelPath);

#if ACTIVATE_TOOL
	HRESULT														Ready_Bones(const aiNode*, _uint iParentBoneIndex);
	HRESULT														Ready_Animations(const aiScene*);
	HRESULT														Ready_Meshes(const aiScene*, _matrixf mPivot);
	HRESULT														Ready_Materials(const aiScene*, const wstring& wstrModelPath);
#endif

public:
	_bool														Is_AnimationFinished() const;

	_uint														Get_AnimationIndex() const				{ return m_iCurrentAnimationIndex; }
	_uint														Get_BoneIndex(const _char* szBoneName) const;
	_uint														Get_MeshIndex(const _char* szMeshName) const;
	_float4x4													Get_Pivot() const						{ return m_mPivot; }
#if ACTIVATE_TOOL
	const _uint													Get_NumBones() const					{ return m_iNumBones; }
	const _uint													Get_NumAnimations() const				{ return m_iNumAnimations; }
	const _uint													Get_NumMeshes() const					{ return m_iNumMeshes; }
	const _uint													Get_NumMaterials() const				{ return m_iNumMaterials; }

	shared_ptr<class CBone>										Get_Bone(const _char* szBoneName) const	{ return m_vecBones[Get_BoneIndex(szBoneName)]; }
	shared_ptr<class CBone>										Get_Bone(_uint iIndex) const			{ return m_vecBones[iIndex]; }
	shared_ptr<class CAnimation>								Get_Animation(_uint iIndex) const		{ return m_vecAnimations[iIndex]; }
	shared_ptr<class CMesh>										Get_Mesh(const _char* szMeshName) const { return m_vecMeshes[Get_MeshIndex(szMeshName)]; }
	shared_ptr<class CMesh>										Get_Mesh(_uint iIndex) const			{ return m_vecMeshes[iIndex]; }
	MATERIAL													Get_Material(_uint iIndex) const		{ return m_vecMaterials[iIndex]; }
#endif

public:
	void														Tick_Animation(_float fTimeDelta);
	void														Set_Animation(_uint iAnimationIndex, _float fPlaySpeed = 1.f, _bool bReverse = false, _float fInterpolationDuration = g_fDefaultInterpolationDuration, _bool bLoop = true);
	void														Reset_Animation();

	void														Iterate_Meshes(function<_bool(shared_ptr<class CMesh>)>);

	HRESULT														Show_MeshFromAnimations(_uint iAnimationIndex, _uint iMeshIndex);
	HRESULT														Hide_MeshFromAnimations(_uint iAnimationIndex, _uint iMeshIndex);

private:
	HRESULT														Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, _uint iTextureIndex = 0);
	HRESULT														Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const _char* szConstantName, _uint iTextureIndex = 0);
	HRESULT														Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>);
	HRESULT														Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const _char* szConstantName);
	HRESULT														Bind_BoneMatrices(_uint iMeshIndex, shared_ptr<class CShader>, const _char* szConstantName);

private:
	MODEL														m_eType						= MODEL::MAX;
	_float4x4													m_mPivot					= g_mUnit;

	_uint														m_iNumBones					= 0;
	vector<shared_ptr<class CBone>>								m_vecBones;

	_uint														m_iNumAnimations			= 0;
	vector<shared_ptr<class CAnimation>>						m_vecAnimations;

	_uint														m_iNumMeshes				= 0;
	vector<shared_ptr<class CMesh>>								m_vecMeshes;

	_uint														m_iNumMaterials				= 0;
	vector<MATERIAL>											m_vecMaterials;
	vector<MATERIALDESC>										m_vecMaterialDescs;

	_uint														m_iCurrentAnimationIndex	= 0;
	_float														m_fAnimationPlaySpeed		= 1.f;
	_bool														m_bAnimReverse				= false;
	_bool														m_bAnimLoop					= true;

	map<_uint, _flags>											m_mapMeshShaderFlags;
	map<_uint, function<HRESULT(shared_ptr<class CShader>)>>	m_mapMeshShaderBindings;
	multimap<_uint, _uint>										m_mapHideMeshFromAnimations;

public:
	static shared_ptr<CModel>									Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const MODEL, const wstring& wstrModelPath, _matrixf mPivot = g_mUnit);
	virtual shared_ptr<CComponent>								Clone(any mapDesc = g_aNull) override;

#if ACTIVATE_TOOL
	HRESULT														Export(const wstring& wstrPath);
#endif
};

END
