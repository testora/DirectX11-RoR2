#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CChannel final
{
private:
	explicit CChannel() DEFAULT;
	virtual ~CChannel() DEFAULT;

public:
	HRESULT						Initialize(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
	void						Update_Transformation(vector<shared_ptr<class CBone>>, _uint& iCurrentKeyFrame, _float fTrackPosition);

#ifdef _DEBUG
#if ACTIVATE_IMGUI
public:
	_uint						Get_BoneIndex() const				{ return m_iBoneIndex; }
	_uint						Get_NumKeyFrames() const			{ return m_iNumKeyFrames; }
	KEYFRAME					Get_KeyFrame(_uint iIndex) const	{ return m_vecKeyFrames[iIndex]; }
#endif
#endif

private:
	_uint						m_iBoneIndex		= g_iMaxBones;

	_uint						m_iNumKeyFrames		= 0;
	vector<KEYFRAME>			m_vecKeyFrames;

public:
	static shared_ptr<CChannel>	Create(const aiNodeAnim* pAIChannel, shared_ptr<class CModel>);
};

END
