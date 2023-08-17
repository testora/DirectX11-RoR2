#include "EnginePCH.h"
#include "Animation.h"
#include "Channel.h"

HRESULT CAnimation::Initialize(const aiAnimation* _pAIAnimation, shared_ptr<class CModel> _pModel)
{
	HRESULT hr = S_OK;

	m_fDuration			= static_cast<_float>(_pAIAnimation->mDuration);
	m_fTicksPerSecond	= static_cast<_float>(_pAIAnimation->mTicksPerSecond);

	m_iNumChannels		= _pAIAnimation->mNumChannels;
	m_vecChannels.reserve(m_iNumChannels);
	m_vecChannelKeyFrames.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		shared_ptr<CChannel> pChannel = CChannel::Create(_pAIAnimation->mChannels[i], _pModel);
		if (nullptr == pChannel)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CAnimation::Create", "Failed to CChannel::Create");
		}

		m_vecChannels.emplace_back(pChannel);
	}

	return hr;
}

void CAnimation::Update(_float _fTimeDelta, vector<shared_ptr<class CBone>> _pModelBones)
{
	m_fTrackPosition += m_fTicksPerSecond * _fTimeDelta;

	if (m_fTrackPosition > m_fDuration)
	{
		m_fTrackPosition = 0.f;
	}

	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		m_vecChannels[i]->Update_Transformation(_pModelBones, m_vecChannelKeyFrames[i], m_fTrackPosition);
	}
}

shared_ptr<CAnimation> CAnimation::Create(const aiAnimation* _pAIAnimation, shared_ptr<class CModel> _pModel)
{
	shared_ptr<CAnimation> pInstance = make_private_shared(CAnimation);

	if (FAILED(pInstance->Initialize(_pAIAnimation, _pModel)))
	{
		MSG_RETURN(nullptr, "CAnimation::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CAnimation> CAnimation::Clone()
{
	return make_private_shared_copy(CAnimation, *this);
}
