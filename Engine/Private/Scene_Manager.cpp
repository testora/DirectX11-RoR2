#include "EnginePCH.h"
#include "Scene_Manager.h"
#include "Scene.h"

void CScene_Manager::Tick(_float _fTimeDelta)
{
	if (nullptr != m_pScene)
	{
		m_pScene->Tick(_fTimeDelta);
	}
}

void CScene_Manager::Late_Tick(_float _fTimeDelta)
{
	if (nullptr != m_pScene)
	{
		m_pScene->Late_Tick(_fTimeDelta);
	}
}

HRESULT CScene_Manager::Open_Scene(shared_ptr<class CScene> _pScene)
{
	m_pScene = _pScene;

	return S_OK;
}
