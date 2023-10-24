#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight final
{
private:
	explicit CLight() DEFAULT;
	virtual ~CLight() DEFAULT;

public:
	HRESULT						Initialize(const LIGHTDESC, shared_ptr<class CTransform>);
	void						Tick();

public:
	_bool						Is_Expired() const;
	LIGHTDESC					Get_LightDesc() const				{ return m_tLightDesc; }
	_float4x4					Get_ShadowViewMatrix() const		{ return m_mShadowView; }
	_float4x4					Get_ShadowProjectionMatrix() const	{ return m_mShadowProjection; }

public:
	HRESULT						Bind_Light(shared_ptr<class CShader>, shared_ptr<class CVIBuffer_Rect>);
	HRESULT						Bind_ShadowMatrices(shared_ptr<class CShader>);

private:
	LIGHTDESC					m_tLightDesc{};
	weak_ptr<class CTransform>	m_pTransform;

	_float4x4					m_mShadowView;
	_float4x4					m_mShadowProjection;

public:
	static shared_ptr<CLight>	Create(const LIGHTDESC, shared_ptr<class CTransform> = nullptr);

	friend class CLight_Manager;
};

END
