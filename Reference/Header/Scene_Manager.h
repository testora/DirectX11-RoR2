#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CScene_Manager final : public CSingleton<CScene_Manager>
{
private:
	explicit CScene_Manager() DEFAULT;
	virtual ~CScene_Manager() DEFAULT;

public:
	void						Tick(_float fTimeDelta);
	void						Late_Tick(_float fTimeDelta);

public:
	HRESULT						Open_Scene(SCENE, shared_ptr<class CScene>);

private:
	shared_ptr<class CScene>	m_pScene;
	SCENE						m_eScene;

	friend CSingleton<CScene_Manager>;
};

END
