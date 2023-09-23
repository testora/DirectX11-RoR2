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
	HRESULT									Add_RenderObject(const RENDER_GROUP, shared_ptr<class ISystem>);

	HRESULT									Draw_RenderGroup();

private:
	HRESULT									Ready_Camera();

	HRESULT									Render_Priority();
	HRESULT									Render_NonBlend();

	HRESULT									Draw_Light();
	HRESULT									Draw_Deferred();

	HRESULT									Render_NonLight();
	HRESULT									Render_Blend();
	HRESULT									Render_UI();

private:
	list<shared_ptr<class ISystem>>			m_lstRenderGroup[IDX(RENDER_GROUP::MAX)];

	shared_ptr<class CRenderTarget_Manager>	m_pRenderTarget_Manager;
	shared_ptr<class CLight_Manager>		m_pLight_Manager;

	shared_ptr<class CShader>				m_pShader;
	shared_ptr<class CVIBuffer_Rect>		m_pVIBuffer;

	_float4x4								m_mWorld;
	_float4x4								m_mView;
	_float4x4								m_mProjection;

public:
	static shared_ptr<CRenderer>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>			Clone(any = g_aNull) override;
};

END
