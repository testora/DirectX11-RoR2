#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
private:
	explicit CRenderer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CRenderer() DEFAULT;

public:
	HRESULT									Initialize(any = g_aNull);

public:
	HRESULT									Add_RenderObject(const RENDER_GROUP, shared_ptr<class CGameObject>);

	HRESULT									Draw_RenderGroup();

private:
	HRESULT									Ready_Camera();
	HRESULT									Render_Priority();
	HRESULT									Render_NonBlend();
	HRESULT									Render_Blend();
	HRESULT									Render_UI();

private:
	vector<function<HRESULT()>>				m_vecFnRenderGroup;

	list<shared_ptr<class CGameObject>>		m_lstRenderGroup[IDX(RENDER_GROUP::MAX)];

public:
	static shared_ptr<CRenderer>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>			Clone(any = g_aNull) override;
};

END
