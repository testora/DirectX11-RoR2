#include "EnginePCH.h"
#include "Bone.h"

CBone::CBone(_uint _iParentBoneIndex)
	: m_iParentBoneIndex(_iParentBoneIndex)
{
}

HRESULT CBone::Initialize(const aiNode* _pAINode)
{
	strcpy_s(m_szName, _pAINode->mName.data);

	memcpy(&m_mTransformation, &_pAINode->mTransformation, sizeof(_float4x4));

	m_mTransformation			= XMMatrixTranspose(m_mTransformation);
	m_mCombinedTransformation	= m_mTransformation;

	return S_OK;
}

void CBone::Set_Transformation(_vectorf _vScale, _vectorf _vRotation, _vectorf _vTranslation)
{
	m_mTransformation = XMMatrixAffineTransformation(_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), _vRotation, _vTranslation);
}

void CBone::Update_CombinedTransformation(vector<shared_ptr<CBone>> _vecModelBones)
{
	if (m_iParentBoneIndex != g_iMaxBones)
	{
		m_mCombinedTransformation = m_mTransformation * _vecModelBones[m_iParentBoneIndex]->m_mCombinedTransformation;
	}
}

shared_ptr<CBone> CBone::Create(const aiNode* _pAINode, _uint _iParentBoneIndex)
{
	shared_ptr<CBone> pInstance = make_private_shared(CBone, _iParentBoneIndex);

	if (FAILED(pInstance->Initialize(_pAINode)))
	{
		MSG_RETURN(nullptr, "CBone::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CBone> CBone::Clone()
{
	return make_private_shared_copy(CBone, *this);
}
