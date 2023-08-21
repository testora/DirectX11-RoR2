#include "EnginePCH.h"
#include "Scene_Manager.h"
#include "Scene.h"
#include "Object_Manager.h"
#include "Grid_Manager.h"

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

HRESULT CScene_Manager::Initialize(_In_ const SCENE _eStatic, _In_ const SCENE _eMax)
{
	m_eSceneStatic	= _eStatic;
	m_eSceneMax		= _eMax;

	return S_OK;
}

HRESULT CScene_Manager::Open_Scene(SCENE _eScene, shared_ptr<class CScene> _pScene)
{
	CObject_Manager::Get_Instance()->Clear_Scene_Object(m_eScene);
	CGrid_Manager::Get_Instance()->Reset_Grids(m_eScene);

	m_eScene = _eScene;
	m_pScene = _pScene;

	return S_OK;
}
