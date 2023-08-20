#pragma once
#include "Engine_Define.h"
#include "Scene.h"

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
	HRESULT						Initialize(_In_ const SCENE eStatic, _In_ const SCENE eMax);

public:
	const SCENE					Current_Scene(_bool bSkipLoad = true) const	{ return m_pScene ? m_pScene->Get_Scene(bSkipLoad) : m_eSceneStatic; }
	const SCENE					Static_Scene() const						{ return m_eSceneStatic; }
	const SCENE					Max_Scene() const							{ return m_eSceneMax; }

public:
	HRESULT						Open_Scene(SCENE, shared_ptr<class CScene>);

private:
	shared_ptr<class CScene>	m_pScene;
	SCENE						m_eScene		= static_cast<SCENE>(0);

	SCENE						m_eSceneStatic	= static_cast<SCENE>(0);
	SCENE						m_eSceneMax		= static_cast<SCENE>(0);

	friend CSingleton<CScene_Manager>;
};

END
