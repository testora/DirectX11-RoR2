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
	virtual HRESULT						Initialize(const char* pModelPath, _matrixf mPivot = XMMatrixIdentity());
	HRESULT								Render(_uint iMeshIndex);

public:
	HRESULT								Bind_ShaderResourceView(shared_ptr<class CShader>, _uint iPassIndex, aiTextureType, const char* pConstantName, _uint iTextureIdx = 0);
	HRESULT								Bind_ShaderResourceView(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const char* pConstantName, _uint iTextureIdx = 0);
	HRESULT								Bind_ShaderResourceViews(shared_ptr<class CShader>, _uint iPassIndex, aiTextureType, const char* pConstantName);
	HRESULT								Bind_ShaderResourceViews(_uint iMeshIndex, shared_ptr<class CShader>, aiTextureType, const char* pConstantName);

private:
	HRESULT								Ready_Meshes(_matrixf mPivot);
	HRESULT								Ready_Materials(const char* pModelPath);

private:
	Assimp::Importer					m_Importer;
	const aiScene*						m_pAIScene		= nullptr;

	const TYPE							m_eType			= TYPE::MAX;

	_uint								m_iNumMeshes	= 0;
	vector<shared_ptr<class CMesh>>		m_vecMeshes;

	_uint								m_iNumMaterials	= 0;
	vector<MODELMATERIAL>				m_vecMaterials;

public:
	static shared_ptr<CModel>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const TYPE, const char* pModelFilePath, _matrixf mPivot = XMMatrixIdentity());
	virtual shared_ptr<CComponent>		Clone(any = any()) override;
};

END
