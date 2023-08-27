#include "EnginePCH.h"
#include "Channel.h"
#include "Model.h"
#include "Bone.h"

HRESULT CChannel::Initialize(const aiNodeAnim* _pAIChannel, shared_ptr<CModel> _pModel)
{
	m_iBoneIndex	= _pModel->Get_BoneIndex(_pAIChannel->mNodeName.data);
	m_iNumKeyFrames	= Function::Max(_pAIChannel->mNumScalingKeys, _pAIChannel->mNumRotationKeys, _pAIChannel->mNumPositionKeys);

	_float3	vScale;
	_float4	vRotation;
	_float3	vTranslation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME tKeyFrame{};

		if (i < _pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &_pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			tKeyFrame.fTime = static_cast<_float>(_pAIChannel->mScalingKeys[i].mTime);
		}

		if (i < _pAIChannel->mNumRotationKeys)
		{
			vRotation.x		= _pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y		= _pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z		= _pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w		= _pAIChannel->mRotationKeys[i].mValue.w;
			tKeyFrame.fTime	= static_cast<_float>(_pAIChannel->mRotationKeys[i].mTime);
		}

		if (i < _pAIChannel->mNumPositionKeys)
		{
			memcpy(&vTranslation, &_pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			tKeyFrame.fTime = static_cast<_float>(_pAIChannel->mPositionKeys[i].mTime);
		}

		memcpy(&tKeyFrame.vScale, &vScale, sizeof(_float3));
		memcpy(&tKeyFrame.vRotation, &vRotation, sizeof(_float4));
		memcpy(&tKeyFrame.vTranslation, &vTranslation, sizeof(_float3));
		tKeyFrame.vTranslation.w = 1.f;

		m_vecKeyFrames.push_back(tKeyFrame);
	}
		
	return S_OK;
}

void CChannel::Update_Transformation(vector<shared_ptr<CBone>> _vecModelBones, _uint& _iCurrentKeyFrame, _float _fTrackPosition)
{
	if (0.f == _fTrackPosition)
	{
		_iCurrentKeyFrame = 0;
	}
	
	_float	fRatio;
	_vector	vScale, vRotation, vTranslation;

	if (_fTrackPosition < m_vecKeyFrames.back().fTime)
	{
		while (_fTrackPosition > m_vecKeyFrames[_iCurrentKeyFrame + 1].fTime)
		{
			++_iCurrentKeyFrame;
		}

		fRatio			= Function::ProportionalRatio(m_vecKeyFrames[_iCurrentKeyFrame].fTime, m_vecKeyFrames[_iCurrentKeyFrame + 1].fTime, _fTrackPosition);

		vScale			= XMVectorLerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vScale), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vScale), fRatio);
		vRotation		= XMQuaternionSlerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vRotation), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vRotation), fRatio);
		vTranslation	= XMVectorLerp(_float4(m_vecKeyFrames[_iCurrentKeyFrame].vTranslation), _float4(m_vecKeyFrames[_iCurrentKeyFrame + 1].vTranslation), fRatio);
	}
	else
	{
		vScale			= _float4(m_vecKeyFrames.back().vScale);
		vRotation		= _float4(m_vecKeyFrames.back().vRotation);
		vTranslation	= _float4(m_vecKeyFrames.back().vTranslation);
	}

	_vecModelBones[m_iBoneIndex]->Set_Transformation(vScale, vRotation, vTranslation);
}

shared_ptr<CChannel> CChannel::Create(const aiNodeAnim* _pAIChannel, shared_ptr<CModel> _pModel)
{
	shared_ptr<CChannel> pInstance = make_private_shared(CChannel);

	if (FAILED(pInstance->Initialize(_pAIChannel, _pModel)))
	{
		MSG_RETURN(nullptr, "CChannel::Create", "Failed to Initialize");
	}

	return pInstance;
}
