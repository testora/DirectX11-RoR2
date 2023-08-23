#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent, public std::enable_shared_from_this<CModel>
{
private:
	explicit CModel(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const MODEL);
	explicit CModel(const CModel&);
	virtual ~CModel() DEFAULT;

public:
	virtual HRESULT							Initialize(const char* pModelPath, _matrixf mPivot = g_mUnit);
	HRESULT									Render(shared_ptr<class CShader>, _uint iPassIndex);
	HRESULT									Render(_uint iMeshIndex, shared_ptr<class CShader>, _uint iPassIndex);

public:
	_uint									Get_NumMeshes() const	{ return m_iNumMeshes; }
	_uint									Get_BoneIndex(const char* pBoneName);

public:
	void									Tick_Animation(_float fTimeDelta);

	HRESULT									Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, _uint iTextureIdx = 0);
	HRESULT									Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const char* pConstantName, _uint iTextureIdx = 0);
	HRESULT									Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>);
	HRESULT									Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const char* pConstantName);
	HRESULT									Bind_BoneMatrices(_uint iMeshIndex, shared_ptr<class CShader>, const char* pConstantName);

	void									Iterate_Meshes(function<_bool(shared_ptr<class CMesh>)>);

private:
	HRESULT									Ready_Meshes(_matrixf mPivot);
	HRESULT									Ready_Materials(const char* pModelPath);
	HRESULT									Ready_Bones(const aiNode* pAINode, _uint iParentBoneIndex);
	HRESULT									Ready_Animations();

private:
	Assimp::Importer						m_Importer;
	const aiScene*							m_pAIScene			= nullptr;

	const MODEL								m_eType				= MODEL::MAX;

	_uint									m_iNumMeshes		= 0;
	vector<shared_ptr<class CMesh>>			m_vecMeshes;

	_uint									m_iNumMaterials		= 0;
	vector<MODELMATERIAL>					m_vecMaterials;

	vector<shared_ptr<class CBone>>			m_vecBones;

	_uint									m_iCurrentAnimIdx	= 0;
	_uint									m_iNumAnimations	= 0;
	vector<shared_ptr<class CAnimation>>	m_vecAnimations;

public:
	static shared_ptr<CModel>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const MODEL, const char* pModelFilePath, _matrixf mPivot = g_mUnit);
	virtual shared_ptr<CComponent>			Clone(any = any()) override;
};

END
