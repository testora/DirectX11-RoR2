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
#if ACTIVATE_TOOL
	HRESULT								Initialize_FromAssimp(const aiAnimation* pAIAnimation, shared_ptr<class CModel>);
#endif
	HRESULT								Initialize_FromBinary(std::ifstream&);
	void								Tick(_float fTimeDelta, vector<shared_ptr<class CBone>>::iterator itBegin, _bool bLoop = true);

public:
	_bool								Is_Finished() const	{ return m_fTrackPosition > m_fDuration; }

	const _char*						Get_Name() const	{ return m_szName; }

private:
	_char								m_szName[MAX_PATH]	= "";

	_float								m_fDuration			= 0.f;
	_float								m_fTicksPerSecond	= 0.f;
	_float								m_fTrackPosition	= 0.f;

	_uint								m_iNumChannels		= 0;
	vector<shared_ptr<class CChannel>>	m_vecChannels;
	vector<_uint>						m_vecChannelKeyFrames;

public:
#if ACTIVATE_TOOL
	static shared_ptr<CAnimation>		Create(const aiAnimation* pAIAnimation, shared_ptr<class CModel>);
#endif
	static shared_ptr<CAnimation>		Read(std::ifstream&);
	shared_ptr<CAnimation>				Clone();

#if ACTIVATE_TOOL
	void								Export(std::ofstream&);
#endif
};

END
