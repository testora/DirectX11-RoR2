#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CScene_Manager final : public CSingleton<CScene_Manager>
{
private:
	explicit CScene_Manager();
	virtual ~CScene_Manager() DEFAULT;

public:
	HRESULT		Open_Scene(SCENE eScene);
	HRESULT		Tick(_float fTimeDelta);
	HRESULT		Late_Tick(_float fTimeDelta);

	friend CSingleton<CScene_Manager>;
};

END
