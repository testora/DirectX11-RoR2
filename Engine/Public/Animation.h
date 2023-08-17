#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final
{
private:
	explicit CAnimation() DEFAULT;
	explicit CAnimation(const CAnimation&) DEFAULT;
	virtual ~CAnimation() DEFAULT;

public:
	HRESULT								Initialize(const aiAnimation* pAIAnimation, shared_ptr<class CModel>);
	void								Update(_float fTimeDelta, vector<shared_ptr<class CBone>>);

private:
	_float								m_fDuration			= 0.f;
	_float								m_fTicksPerSecond	= 0.f;
	_float								m_fTrackPosition	= 0.f;

	_uint								m_iNumChannels		= 0;
	vector<shared_ptr<class CChannel>>	m_vecChannels;
	vector<_uint>						m_vecChannelKeyFrames;

public:
	static shared_ptr<CAnimation>		Create(const aiAnimation* pAIAnimation, shared_ptr<class CModel>);
	shared_ptr<CAnimation>				Clone();
};

END
