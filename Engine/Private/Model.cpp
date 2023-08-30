#include "EnginePCH.h"
#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Animation.h"
#include "Texture.h"
#include "Shader.h"

CModel::CModel(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const MODEL _eType)
	: CComponent(_pDevice, _pContext, COMPONENT::MODEL)
	, m_eType	(_eType)
{
}

CModel::CModel(const CModel& _rhs)
	: CComponent		(_rhs)
	, m_eType			(_rhs.m_eType)
	, m_iNumMeshes		(_rhs.m_iNumMeshes)
	, m_iNumMaterials	(_rhs.m_iNumMaterials)
	, m_iNumAnimations	(_rhs.m_iNumAnimations)
	, m_vecMeshes		(_rhs.m_vecMeshes)
	, m_vecMaterials	(_rhs.m_vecMaterials)
	, m_iCurrentAnimIdx	(_rhs.m_iCurrentAnimIdx)
{
	for (auto pOriginal : _rhs.m_vecAnimations)
	{
		m_vecAnimations.push_back(pOriginal->Clone());
	}

	for (auto pOriginal : _rhs.m_vecBones)
	{
		m_vecBones.push_back(pOriginal->Clone());
	}
}

HRESULT CModel::Initialize(const wstring& _wstrModelPath, _matrixf _mPivot)
{
	wstring strExt(_wstrModelPath.substr(_wstrModelPath.rfind(L'.')));

	if (strExt == TEXT(".mdl"))
	{
		if (FAILED(Initialize_FromBinary(_wstrModelPath)))
		{
			MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Initialize_FromBinary");
		}
	}
#if ACTIVATE_TOOL
	else if (strExt == TEXT(".fbx"))
	{
		if (FAILED(Initialize_FromAssimp(_wstrModelPath, _mPivot)))
		{
			MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Intialize_FromAssimp");
		}
	}
#endif
	else
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize", "Invalid Extension");
	}

	return S_OK;
}

HRESULT CModel::Render(shared_ptr<CShader> _pShader, _uint _iPassIndex)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (FAILED(Bind_ShaderResourceViews(i, _pShader)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CModel::Bind_ShaderResourceViews");
		}

		if (FAILED(Bind_BoneMatrices(i, _pShader, SHADER_BONE)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CModel::Bind_BoneMatrices");
		}

		if (FAILED(Render(i, _pShader, _iPassIndex)))
		{
			MSG_RETURN(E_FAIL, "CGameObject::Render", "Failed to CModel::Render");
		}
	}

	return S_OK;
}

_uint CModel::Get_BoneIndex(const _char* _szBoneName)
{
	for (_uint i = 0; i < m_vecBones.size(); ++i)
	{
		if (!strcmp(m_vecBones[i]->Get_Name(), _szBoneName))
		{
			return i;
		}
	}

	return static_cast<_uint>(m_vecBones.size());
}

HRESULT CModel::Render(_uint _iMeshIndex, shared_ptr<CShader> _pShader, _uint _iPassIndex)
{
	HRESULT hr = S_OK;

	if (m_vecMeshes[_iMeshIndex])
	{
		if (FAILED(m_vecMeshes[_iMeshIndex]->Render(_pShader, _iPassIndex)))
		{
			MSG_BOX("CModel::Render", "Failed to CMesh::Render");
			hr = E_FAIL;
		}
	}

	return hr;
}

#if ACTIVATE_TOOL
HRESULT CModel::Initialize_FromAssimp(const wstring& _wstrModelPath, _matrixf _mPivot)
{
	Assimp::Importer	aiImporter;
	const aiScene*		pAIScene	= nullptr;
	
	_uint iAIFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;
	if (MODEL::NONANIM == m_eType)
	{
		iAIFlag |= aiProcess_PreTransformVertices;
	}

	pAIScene = aiImporter.ReadFile(Function::ToString(_wstrModelPath).c_str(), iAIFlag);

	if (nullptr == pAIScene)
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to ReadFile");
	}

	if (FAILED(Ready_Bones(pAIScene->mRootNode, g_iMaxBones)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Bones");
	}
	m_iNumBones = static_cast<_uint>(m_vecBones.size());

	if (FAILED(Ready_Animations(pAIScene)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Animations");
	}

	if (FAILED(Ready_Meshes(pAIScene, _mPivot)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Meshes");
	}

	if (FAILED(Ready_Materials(pAIScene, _wstrModelPath)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromAssimp", "Failed to Ready_Materials");
	}

	return S_OK;
}
#endif

HRESULT CModel::Initialize_FromBinary(const wstring& _wstrModelPath)
{
	std::ifstream inFile(_wstrModelPath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Open File");
	}

	inFile.read(reinterpret_cast<_byte*>(&m_eType),				sizeof(MODEL));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumBones),			sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumAnimations),	sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumMeshes),		sizeof(_uint));
	inFile.read(reinterpret_cast<_byte*>(&m_iNumMaterials),		sizeof(_uint));
	
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Model");
	}

	m_vecBones.reserve(m_iNumBones);
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		m_vecBones.emplace_back(CBone::Read(inFile));
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Bones");
	}

	m_vecAnimations.reserve(m_iNumAnimations);
	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		m_vecAnimations.emplace_back(CAnimation::Read(inFile));
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Animations");
	}

	m_vecMeshes.reserve(m_iNumMeshes);
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		m_vecMeshes.emplace_back(CMesh::Read(m_pDevice, m_pContext, m_eType, inFile));
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Meshes");
	}

	m_vecMaterials.resize(m_iNumMaterials);
	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			size_t	nNumTextures(0);
			inFile.read(reinterpret_cast<_byte*>(&nNumTextures),		sizeof(size_t));

			m_vecMaterials[i].pTexture[j] = CTexture::Create(m_pDevice, m_pContext, static_cast<_uint>(nNumTextures));

			for (size_t k = 0; k < nNumTextures; ++k)
			{
				size_t	nPathLength(0);
				inFile.read(reinterpret_cast<_byte*>(&nPathLength),		sizeof(size_t));

				wstring	wstrPath;
				wstrPath.resize(nPathLength);
				inFile.read(reinterpret_cast<_byte*>(wstrPath.data()),	sizeof(_wchar) * nPathLength);

				m_vecMaterials[i].pTexture[j]->Push_ShaderResourceView(wstrPath);
			}
		}
	}
	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CModel::Initialize_FromBinary", "Failed to Read File: Materials");
	}

	inFile.close();

	return S_OK;
}

#if ACTIVATE_TOOL
HRESULT CModel::Ready_Bones(const aiNode* _pAINode, _uint _iParentBoneIndex)
{
	shared_ptr<CBone> pBone = CBone::Create(_pAINode, _iParentBoneIndex);
	if (nullptr == pBone)
	{
		MSG_RETURN(E_FAIL, "CModel::Ready_Bones", "Failed to CBone::Create");
	}

	m_vecBones.emplace_back(pBone);

	for (size_t i = 0; i < _pAINode->mNumChildren; ++i)
	{
		if (FAILED(Ready_Bones(_pAINode->mChildren[i], Get_BoneIndex(pBone->Get_Name()))))
		{
			MSG_RETURN(E_FAIL, "CModel::Ready_Bones", "Failed to Ready_Bones");
		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations(const aiScene* _pAIScene)
{
	HRESULT hr = S_OK;

	m_iNumAnimations = _pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		shared_ptr<CAnimation> pAnimation = CAnimation::Create(_pAIScene->mAnimations[i], static_pointer_cast<CModel>(shared_from_this()));
		if (nullptr == pAnimation)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CModel::Ready_Animations", "Failed to CAnimation::Create");
		}

		m_vecAnimations.emplace_back(pAnimation);
	}

	return hr;
}

HRESULT CModel::Ready_Meshes(const aiScene* _pAIScene, _matrixf _mPivot)
{
	HRESULT hr = S_OK;

	m_iNumMeshes = _pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		shared_ptr<CMesh> pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, _pAIScene->mMeshes[i], static_pointer_cast<CModel>(shared_from_this()), _mPivot);
		if (nullptr == pMesh)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CModel::Ready_Meshes", "Failed to CMesh::Create");
		}

		m_vecMeshes.emplace_back(pMesh);
	}

	return hr;
}

HRESULT CModel::Ready_Materials(const aiScene* _pAIScene, const wstring& _wstrModelPath)
{
	HRESULT hr = S_OK;

	m_iNumMaterials = _pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MATERIAL	tModelMaterial{};
		aiMaterial*	pAIMaterial = _pAIScene->mMaterials[i];

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			shared_ptr<CTexture> pTexture = CTexture::Create(m_pDevice, m_pContext, _wstrModelPath, pAIMaterial, aiTextureType(j));
			if (nullptr == pTexture)
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Ready_Materials", "Failed to CTexture::Create");
			}

			tModelMaterial.pTexture[j] = pTexture;
		}

		m_vecMaterials.emplace_back(tModelMaterial);
	}

	return hr;
}
#endif

void CModel::Tick_Animation(_float _fTimeDelta)
{
	if (MODEL::NONANIM == m_eType)
	{
		return;
	}

	m_vecAnimations[m_iCurrentAnimIdx]->Tick(_fTimeDelta, m_vecBones, m_bAnimLoop);

	for (auto pBone : m_vecBones)
	{
		pBone->Update_CombinedTransformation(m_vecBones);
	}
}

void CModel::Set_Animation(_uint _iAnimIdx, _float _fInterpolationDuration, _bool _bLoop)
{
	if (_iAnimIdx >= m_iNumAnimations)
	{
		MSG_RETURN(, "CModel::Set_Animation", "Invalid Index");
	}

	for (auto pMesh : m_vecMeshes)
	{
		pMesh->Set_Interpolation(m_vecBones, _fInterpolationDuration);
	}

	m_iCurrentAnimIdx	= _iAnimIdx;
	m_bAnimLoop			= _bLoop;
}

void CModel::Iterate_Meshes(function<_bool(shared_ptr<CMesh>)> _funcCallback)
{
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		if (!_funcCallback(m_vecMeshes[i]))
		{
			return;
		}
	}
}

HRESULT CModel::Bind_ShaderResourceView(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, _uint _iTextureIdx)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}
	
	HRESULT hr = S_OK;

	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		if (nullptr == m_vecMaterials[iMaterialIndex].pTexture[i])
		{
			continue;
		}

		switch (i)
		{
		case aiTextureType_DIFFUSE:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceView(_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE, _iTextureIdx)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceView", "Failed to CTexture::Bind_ShaderResourceView")
			}
			break;

		case aiTextureType_NORMALS:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceView(_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL, _iTextureIdx)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceView", "Failed to CTexture::Bind_ShaderResourceView")
			}
			break;
		}
	}

	return hr;
}

HRESULT CModel::Bind_ShaderResourceView(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, aiTextureType _eAIType, const _char* _szConstantName, _uint _iTextureIdx)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	shared_ptr<CTexture> pTexture = m_vecMaterials[iMaterialIndex].pTexture[_eAIType];
	if (nullptr == pTexture)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return pTexture->Bind_ShaderResourceView(_pShader, _eAIType, _szConstantName, _iTextureIdx);
}

HRESULT CModel::Bind_ShaderResourceViews(_uint _iMeshIndex, shared_ptr<class CShader> _pShader)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceViews", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceViews", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceViews", "Invalid Range");
	}

	HRESULT hr = S_OK;

	for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		if (nullptr == m_vecMaterials[iMaterialIndex].pTexture[i])
		{
			continue;
		}

		switch (i)
		{
		case aiTextureType_DIFFUSE:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceViews(_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceViews", "Failed to CTexture::Bind_ShaderResourceViews")
			}
			break;

		case aiTextureType_NORMALS:
			if (FAILED(m_vecMaterials[iMaterialIndex].pTexture[i]->Bind_ShaderResourceViews(_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceViews", "Failed to CTexture::Bind_ShaderResourceViews")
			}
			break;
		}
	}

	return hr;
}

HRESULT CModel::Bind_ShaderResourceViews(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, aiTextureType _eAIType, const _char* _szConstantName)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	_uint iMaterialIndex = m_vecMeshes[_iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	shared_ptr<CTexture> pTexture = m_vecMaterials[iMaterialIndex].pTexture[_eAIType];
	if (nullptr == pTexture)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return pTexture->Bind_ShaderResourceViews(_pShader, _eAIType, _szConstantName);
}

HRESULT CModel::Bind_BoneMatrices(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, const _char* _szConstantName)
{
	if (_iMeshIndex >= m_iNumMeshes)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Invalid Range");
	}

	if (nullptr == m_vecMeshes[_iMeshIndex])
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return _pShader->Bind_MatrixArray(_szConstantName, m_vecMeshes[_iMeshIndex]->Get_BoneMatrices(m_vecBones), g_iMaxBones);
}

shared_ptr<CModel> CModel::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const MODEL _eType, const wstring& _wstrModelPath, _matrixf _mPivot)
{
	shared_ptr<CModel> pInstance = make_private_shared(CModel, _pDevice, _pContext, _eType);

	if (FAILED(pInstance->Initialize(_wstrModelPath, _mPivot)))
	{
		MSG_RETURN(nullptr, "CModel::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CModel::Clone(any)
{
	return make_private_shared_copy(CModel, *this);
}

#if ACTIVATE_TOOL
HRESULT CModel::Export(const wstring& _wstrPath)
{
	std::ofstream outFile(_wstrPath, std::ios::binary);
	if (!outFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Open File");
	}

#pragma region Model
	outFile.write(reinterpret_cast<const _byte*>(&m_eType),				sizeof(MODEL));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumBones),			sizeof(_uint));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumAnimations),	sizeof(_uint));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumMeshes),		sizeof(_uint));
	outFile.write(reinterpret_cast<const _byte*>(&m_iNumMaterials),		sizeof(_uint));

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Model");
	}
#pragma endregion
#pragma region Bones
	for(auto& pBone : m_vecBones)
	{
		pBone->Export(outFile);
	}

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Bones");
	}
#pragma endregion
#pragma region Animations
	for(auto& pAnimation : m_vecAnimations)
	{
		pAnimation->Export(outFile);
	}

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Animations");
	}
#pragma endregion
#pragma region Meshes
	for (auto& pMesh : m_vecMeshes)
	{
		pMesh->Export(outFile, m_eType);
	}

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Meshes");
	}
#pragma endregion
#pragma region Materials
	for (auto& tMaterial : m_vecMaterials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		{
			tMaterial.pTexture[i]->Export(outFile);
		}
	}

	if (outFile.fail())
	{
		outFile.clear();
		outFile.close();
		MSG_RETURN(E_FAIL, "CModel::Export", "Failed to Write File: Materials");
	}
#pragma endregion

	outFile.close();

	return S_OK;
}
#endif
