#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CScene abstract
{
protected:
	explicit CScene(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const SCENE);
	virtual ~CScene() DEFAULT;

public:
	virtual HRESULT												Initialize();
	virtual void												Tick(_float fTimeDelta);
	virtual void												Late_Tick(_float fTimeDelta);
	virtual HRESULT												Render();

public:
	shared_ptr<class CObjectLayer>								Find_Layer(const wstring& strLayerTag);
	shared_ptr<class CObjectPool>								Find_Pool(const type_index);

public:
	HRESULT														Add_Layer(const wstring& strLayerTag);
	HRESULT														Add_Pool(const type_index _type, shared_ptr<class CGameObject>, _uint iPoolSize);

protected:
	unordered_map<wstring, shared_ptr<class CObjectLayer>>		m_umapLayer;
	unordered_map<type_index, shared_ptr<class CObjectPool>>	m_umapPool;

	const SCENE													m_eScene;

private:
	ComPtr<ID3D11Device>										m_pDevice;
	ComPtr<ID3D11DeviceContext>									m_pContext;
};

END
