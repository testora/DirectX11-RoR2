#include "EnginePCH.h"
#include "Scene.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#include "Factory.h"

CScene::CScene(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const SCENE _eScene)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
	, m_eScene(_eScene)
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

shared_ptr<class CObjectLayer> CScene::Find_Layer(const wstring& _strLayerTag)
{
	auto iter = m_umapLayer.find(_strLayerTag);
	if (iter == m_umapLayer.end())
	{
		return nullptr;
	}

	return m_umapLayer[_strLayerTag];
}

shared_ptr<class CObjectPool> CScene::Find_Pool(const type_index _type)
{
	auto iter = m_umapPool.find(_type);
	if (iter == m_umapPool.end())
	{
		return nullptr;
	}

	return m_umapPool[_type];
}

HRESULT CScene::Add_Layer(const wstring& _strLayerTag)
{
	if (nullptr != Find_Layer(_strLayerTag))
	{
		MSG_RETURN(E_FAIL, "CScene::Add_Layer", "Already Exists: CObjectLayer");
	}

	m_umapLayer.emplace(_strLayerTag, CFactory::Create<CObjectLayer>());

	return S_OK;
}

HRESULT CScene::Add_Pool(const type_index _type, shared_ptr<class CGameObject> _pObject, _uint _iPoolSize)
{
	if (nullptr != Find_Pool(_type))
	{
		MSG_RETURN(E_FAIL, "CScene::Add_Pool", "Already Exists: CObjectPool");
	}

	m_umapPool.emplace(_type, CFactory::Create<CObjectPool>(_iPoolSize, _pObject));

	return S_OK;
}
