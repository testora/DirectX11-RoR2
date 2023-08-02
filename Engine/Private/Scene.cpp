#include "EnginePCH.h"
#include "Scene.h"
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

void CScene::Tick(_float _fTimeDelta)
{
}

void CScene::Late_Tick(_float _fTimeDelta)
{
}

HRESULT CScene::Render()
{
	return S_OK;
}
