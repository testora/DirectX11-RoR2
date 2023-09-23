#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CLight_Manager final : public CSingleton<CLight_Manager>
{
private:
	explicit CLight_Manager() DEFAULT;
	virtual ~CLight_Manager() DEFAULT;

public:
	HRESULT											Reserve_Manager(const SCENE);

	void											Tick();

public:
	HRESULT											Add_Light(const SCENE, LIGHTDESC, shared_ptr<class CTransform>);

	HRESULT											Clear_Lights(const SCENE);
	HRESULT											Bind_Lights(shared_ptr<class CShader>, shared_ptr<class CVIBuffer_Rect>);

private:
	typedef vector<shared_ptr<class CLight>>		Lights;
	unique_ptr<Lights[]>							m_arrLights;

	_uint 											m_iLightCount	= 0;

	SCENE											m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CLight_Manager>;
};

END
