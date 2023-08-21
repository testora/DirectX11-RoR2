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
	void											Late_Tick();

public:
	HRESULT											Add_Shaders(shared_ptr<class CShader>);
	HRESULT											Add_Lights(const SCENE, LIGHTDESC, shared_ptr<class CTransform>);

	HRESULT											Clear_Lights(const SCENE);

private:
	HRESULT											Bind_Lights();

private:
	typedef vector<shared_ptr<class CLight>>		Lights;
	unique_ptr<Lights[]>							m_arrLights;

	vector<shared_ptr<class CShader>>				m_vecShaders;

	_uint 											m_iLightCount	= 0;
	array<LIGHTDESC::LIGHTTYPE, g_iMaxLights>		m_arrLightType;
	array<_float4, g_iMaxLights>					m_arrPosition;
	array<_float4, g_iMaxLights>					m_arrDirection;
	array<_color, g_iMaxLights>						m_arrDiffuse;
	array<_color, g_iMaxLights>						m_arrSpecular;
	array<_color, g_iMaxLights>						m_arrAmbient;
	array<_float, g_iMaxLights>						m_arrRange;
	array<_float, g_iMaxLights>						m_arrAttenuation0;
	array<_float, g_iMaxLights>						m_arrAttenuation1;
	array<_float, g_iMaxLights>						m_arrAttenuation2;

	SCENE											m_eSceneMax	= static_cast<SCENE>(0);

	friend CSingleton<CLight_Manager>;
};

END
