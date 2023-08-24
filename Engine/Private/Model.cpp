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

HRESULT CModel::Initialize(const char* _szModelPath, _matrixf _mPivot)
{
	_uint iAIFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;
	if (MODEL::NONANIM == m_eType)
	{
		iAIFlag |= aiProcess_PreTransformVertices;
	}

	m_pAIScene = m_aiImporter.ReadFile(_szModelPath, iAIFlag);
	if (nullptr == m_pAIScene)
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to ReadFile");
	}

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, g_iMaxBones)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Ready_Bones");
	}

	if (FAILED(Ready_Meshes(_mPivot)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Ready_Meshes");
	}

	if (FAILED(Ready_Materials(_szModelPath)))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Ready_Materials");
	}

	if (FAILED(Ready_Animations()))
	{
		MSG_RETURN(E_FAIL, "CModel::Initialize", "Failed to Ready_Animations");
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

_uint CModel::Get_BoneIndex(const char* _szBoneName)
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

void CModel::Tick_Animation(_float _fTimeDelta)
{
	if (MODEL::NONANIM == m_eType)
	{
		return;
	}

	m_vecAnimations[m_iCurrentAnimIdx]->Update(_fTimeDelta, m_vecBones);

	for (auto pBone : m_vecBones)
	{
		pBone->Update_CombinedTransformation(m_vecBones);
	}
}

void CModel::Iterate_Meshes(function<_bool(shared_ptr<CMesh>)> _fn)
{
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		if (_fn(m_vecMeshes[i]))
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
		if (nullptr == m_vecMaterials[iMaterialIndex].pMaterial[i])
		{
			continue;
		}

		switch (i)
		{
		case aiTextureType_DIFFUSE:
			if (FAILED(m_vecMaterials[iMaterialIndex].pMaterial[i]->Bind_ShaderResourceView(_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE, _iTextureIdx)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceView", "Failed to CTexture::Bind_ShaderResourceView")
			}
			break;

		case aiTextureType_NORMALS:
			if (FAILED(m_vecMaterials[iMaterialIndex].pMaterial[i]->Bind_ShaderResourceView(_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL, _iTextureIdx)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceView", "Failed to CTexture::Bind_ShaderResourceView")
			}
			break;
		}
	}

	return hr;
}

HRESULT CModel::Bind_ShaderResourceView(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, aiTextureType _eAIType, const char* _szConstantName, _uint _iTextureIdx)
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

	shared_ptr<CTexture> pMaterial = m_vecMaterials[iMaterialIndex].pMaterial[_eAIType];
	if (nullptr == pMaterial)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return pMaterial->Bind_ShaderResourceView(_pShader, _eAIType, _szConstantName, _iTextureIdx);
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
		if (nullptr == m_vecMaterials[iMaterialIndex].pMaterial[i])
		{
			continue;
		}

		switch (i)
		{
		case aiTextureType_DIFFUSE:
			if (FAILED(m_vecMaterials[iMaterialIndex].pMaterial[i]->Bind_ShaderResourceViews(_pShader, aiTextureType_DIFFUSE, SHADER_TEXDIFFUSE)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceViews", "Failed to CTexture::Bind_ShaderResourceViews")
			}
			break;

		case aiTextureType_NORMALS:
			if (FAILED(m_vecMaterials[iMaterialIndex].pMaterial[i]->Bind_ShaderResourceViews(_pShader, aiTextureType_NORMALS, SHADER_TEXNORMAL)))
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Bind_ShaderResourceViews", "Failed to CTexture::Bind_ShaderResourceViews")
			}
			break;
		}
	}

	return hr;
}

HRESULT CModel::Bind_ShaderResourceViews(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, aiTextureType _eAIType, const char* _szConstantName)
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

	shared_ptr<CTexture> pMaterial = m_vecMaterials[iMaterialIndex].pMaterial[_eAIType];
	if (nullptr == pMaterial)
	{
		MSG_RETURN(E_FAIL, "CModel::Bind_ShaderResourceView", "Null Exception");
	}

	return pMaterial->Bind_ShaderResourceViews(_pShader, _eAIType, _szConstantName);
}

HRESULT CModel::Bind_BoneMatrices(_uint _iMeshIndex, shared_ptr<class CShader> _pShader, const char* _szConstantName)
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

HRESULT CModel::Ready_Meshes(_matrixf _mPivot)
{
	HRESULT hr = S_OK;

	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		shared_ptr<CMesh> pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAIScene->mMeshes[i], static_pointer_cast<CModel>(shared_from_this()), _mPivot);
		if (nullptr == pMesh)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CModel::Ready_Meshes", "Failed to CMesh::Create");
		}

		m_vecMeshes.emplace_back(pMesh);
	}

	return hr;
}

HRESULT CModel::Ready_Materials(const char* _szModelPath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	HRESULT hr = S_OK;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MODELMATERIAL tModelMaterial{};

		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			shared_ptr<CTexture> pMaterial = CTexture::Create(m_pDevice, m_pContext, _szModelPath, pAIMaterial, aiTextureType(j));
			if (nullptr == pMaterial)
			{
				hr = E_FAIL;
				MSG_CONTINUE("CModel::Ready_Materials", "Failed to CTexture::Create");
			}

			tModelMaterial.pMaterial[j] = pMaterial;
		}

		m_vecMaterials.emplace_back(tModelMaterial);
	}

	return hr;
}

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

HRESULT CModel::Ready_Animations()
{
	HRESULT hr = S_OK;

	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		shared_ptr<CAnimation> pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], static_pointer_cast<CModel>(shared_from_this()));
		if (nullptr == pAnimation)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CModel::Ready_Animations", "Failed to CAnimation::Create");
		}

		m_vecAnimations.emplace_back(pAnimation);
	}

	return hr;
}

shared_ptr<CModel> CModel::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const MODEL _eType, const char* _pModelFilePath, _matrixf _mPivot)
{
	shared_ptr<CModel> pInstance = make_private_shared(CModel, _pDevice, _pContext, _eType);

	if (FAILED(pInstance->Initialize(_pModelFilePath, _mPivot)))
	{
		MSG_RETURN(nullptr, "CModel::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CModel::Clone(any)
{
	return make_private_shared_copy(CModel, *this);
}
