#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CChannel final
{
private:
	explicit CChannel() DEFAULT;
	virtual ~CChannel() DEFAULT;

public:
#if ACTIVATE_TOOL
	HRESULT						Initialize_FromAssimp(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
#endif
	HRESULT						Initialize_FromBinary(std::ifstream&);

public:
	_uint						Get_BoneIndex() const							{ return m_iBoneIndex; }
#if ACTIVATE_TOOL
	_uint						Get_NumKeyFrames() const						{ return m_iNumKeyFrames; }

	KEYFRAME					Get_KeyFrame(_uint iIndex)						{ return m_vecKeyFrames[iIndex]; }

	void						Set_KeyFrame(_uint iIndex, KEYFRAME tKeyFrame)	{ m_vecKeyFrames[iIndex] = tKeyFrame; }
#endif

public:
	void						Update_Transformation(vector<shared_ptr<class CBone>>::iterator itBegin, _uint& iCurrentKeyFrame, _float fTrackPosition);
	void						Blend_Transformation(vector<shared_ptr<class CBone>>::iterator itBegin, _uint iKeyFrame, _float fTrackPosition, shared_ptr<class CAnimation> pBase);

private:
	_uint						m_iBoneIndex		= g_iMaxBones;

	_uint						m_iNumKeyFrames		= 0;
	vector<KEYFRAME>			m_vecKeyFrames;

	_float4x4					m_mBase				= g_mUnit;

public:
#if ACTIVATE_TOOL
	static shared_ptr<CChannel>	Create(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
#endif
	static shared_ptr<CChannel>	Read(std::ifstream&);

#if ACTIVATE_TOOL
	void						Export(std::ofstream&);
#endif
};

END
