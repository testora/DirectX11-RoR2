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
	LIGHTDESC					Get_LightDesc() const	{ return m_tLightDesc; }

public:
	HRESULT						Bind_Light(shared_ptr<class CShader>, shared_ptr<class CVIBuffer_Rect>);

private:
	LIGHTDESC					m_tLightDesc{};
	weak_ptr<class CTransform>	m_pTransform;

public:
	static shared_ptr<CLight>	Create(const LIGHTDESC, shared_ptr<class CTransform> = nullptr);

	friend class CLight_Manager;
};

END
