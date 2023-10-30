#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final
{
private:
	explicit CAnimation() DEFAULT;
	explicit CAnimation(const CAnimation&);
	virtual ~CAnimation() DEFAULT;

public:
#if ACTIVATE_TOOL
	HRESULT								Initialize_FromAssimp(const aiAnimation* pAIAnimation, shared_ptr<class CModel>);
#endif
	HRESULT								Initialize_FromBinary(std::ifstream&);
	void								Tick(_float fTimeDelta, vector<shared_ptr<class CBone>>::iterator itBegin, _bool bReverse = false, _bool bLoop = true);


public:
	_bool								Is_ChannelExist(_uint iBoneIndex) const;
	_bool								Is_Finished() const					{ return !m_bReverse ? m_fTrackPosition >= m_fDuration : m_fTrackPosition <= 0.f; }
	_float								Get_Duration() const				{ return m_fDuration; }

#if ACTIVATE_TOOL
	_uint								Get_NumChannels() const				{ return static_cast<_uint>(m_vecChannels.size()); }
#endif

	const _char*						Get_Name() const					{ return m_szName; }
	shared_ptr<class CChannel>			Get_Channel(_uint iChannelIndex, _bool bBoneIndex = false) const;
#if ACTIVATE_TOOL
	_uint								Get_ChannelIndex(_uint iBoneIndex) const;
#endif

public:
	void								Set_AnimationIndex(vector<_uint>::iterator itBegin, _uint iAnimationIndex);

	void								Fix(vector<shared_ptr<class CBone>>::iterator itBegin, _float fRatio);
	void								Blend(vector<shared_ptr<CAnimation>>::iterator itAnimationBegin, vector<shared_ptr<class CBone>>::iterator itBoneBegin, vector<_uint>::iterator itBoneAnimationBegin, _float fRatio);
	void								Reset()								{ m_bReverse = false;	m_fTrackPosition = 0.f; }

#if ACTIVATE_TOOL
	HRESULT								Remove_Channel(_uint iChannelIndex);
#endif

private:
	_char								m_szName[MAX_PATH]					= "";

	_bool								m_bReverse							= false;

	_float								m_fDuration							= 0.f;
	_float								m_fTrackPosition					= 0.f;
	_float								m_fTicksPerSecond					= 0.f;

	_uint								m_iNumChannels						= 0;
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
