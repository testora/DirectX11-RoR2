#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CPipeLine final : public CSingleton<CPipeLine>
{
public:
	enum STATE	{ WORLD, VIEW, PROJ, MAX };

private:
	explicit CPipeLine() DEFAULT;
	virtual ~CPipeLine() DEFAULT;

public:
	_float4x4	Get_Transform(const STATE eState)						{ return m_mTransform[eState]; }

	void		Set_Transform(const STATE eState, _float4x4 mTransform)	{ m_mTransform[eState] = mTransform; }

private:
	_float4x4	m_mTransform[STATE::MAX];

	friend CSingleton<CPipeLine>;
};

END
