#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget final
{
private:
	explicit CRenderTarget() DEFAULT;
	virtual ~CRenderTarget() DEFAULT;

public:
	HRESULT								Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _uint2 vResolution, const DXGI_FORMAT, const _color vClear);

public:
	ComPtr<ID3D11RenderTargetView>		Get_RenderTargetView() const	{ return m_pRenderTargetView; }
#ifdef _DEBUG
	ComPtr<ID3D11ShaderResourceView>	Get_ShaderResourceView() const	{ return m_pShaderResourceView; }
#endif

public:
	void								Clear(ComPtr<ID3D11DeviceContext>);
	HRESULT								Bind_ShaderResourceView(shared_ptr<class CShader>, const _char* szConstantName);

private:
	_color								m_vClear;

	ComPtr<ID3D11Texture2D>				m_pTexture2D;
	ComPtr<ID3D11RenderTargetView>		m_pRenderTargetView;
	ComPtr<ID3D11ShaderResourceView>	m_pShaderResourceView;

public:
	static shared_ptr<CRenderTarget>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _uint2 vResolution, const DXGI_FORMAT, const _color vClear);
};

END
