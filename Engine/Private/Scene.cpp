#include "EnginePCH.h"
#include "Scene.h"
#include "Grid_Manager.h"
#include "Factory.h"

CScene::CScene(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eScene)
	: m_pDevice	(_pDevice)
	, m_pContext(_pContext)
	, m_eScene	(_eScene)
{
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

#ifdef _DEBUG
void CScene::Debug()
{
}
#endif
