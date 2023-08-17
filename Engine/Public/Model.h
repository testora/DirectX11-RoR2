#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent, public std::enable_shared_from_this<CModel>
{
public:
	enum TYPE	{ NONANIM, ANIM, MAX };

private:
	explicit CModel(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const TYPE);
	explicit CModel(const CModel&);
	virtual ~CModel() DEFAULT;

public:
	virtual HRESULT							Initialize(const char* pModelPath, _matrixf mPivot = g_mUnit);
	HRESULT									Render(_uint iMeshIndex);

public:
	_uint									Get_NumMeshes() const	{ return m_iNumMeshes; }
	_uint									Get_BoneIndex(const char* pBoneName);

public:
	void									Play_Animation(_float fTimeDelta);

	HRESULT									Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const char* pConstantName, _uint iTextureIdx = 0);
	HRESULT									Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const char* pConstantName);
	HRESULT									Bind_BoneMatrices(_uint iMeshIndex, shared_ptr<class CShader>, const char* pConstantName);

private:
	HRESULT									Ready_Meshes(_matrixf mPivot);
	HRESULT									Ready_Materials(const char* pModelPath);
	HRESULT									Ready_Bones(const aiNode* pAINode, _uint iParentBoneIndex);
	HRESULT									Ready_Animations();

private:
	Assimp::Importer						m_Importer;
	const aiScene*							m_pAIScene			= nullptr;

	const TYPE								m_eType				= TYPE::MAX;

	_uint									m_iNumMeshes		= 0;
	vector<shared_ptr<class CMesh>>			m_vecMeshes;

	_uint									m_iNumMaterials		= 0;
	vector<MODELMATERIAL>					m_vecMaterials;

	vector<shared_ptr<class CBone>>			m_vecBones;

	_uint									m_iCurrentAnimIdx	= 0;
	_uint									m_iNumAnimations	= 0;
	vector<shared_ptr<class CAnimation>>	m_vecAnimations;

public:
	static shared_ptr<CModel>				Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const TYPE, const char* pModelFilePath, _matrixf mPivot = g_mUnit);
	virtual shared_ptr<CComponent>			Clone(any = any()) override;
};

END
