#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CRenderTarget_Manager final : public CSingleton<CRenderTarget_Manager>
{
private:
	explicit CRenderTarget_Manager() DEFAULT;
	virtual ~CRenderTarget_Manager() DEFAULT;

public:
	HRESULT															Initialize(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);

#ifdef _DEBUG
public:
	ComPtr<ID3D11ShaderResourceView>								Get_ShaderResourceView(const wstring& wstrTargetTag);
#endif

public:
	HRESULT															Push_RenderTarget(const wstring& wstrMultiTargetTag, const wstring& wstrTargetTag);
	HRESULT															Erase_RenderTarget(const wstring& wstrMultiTargetTag, const wstring& wstrTargetTag);

	HRESULT															Add_RenderTarget(const wstring& wstrTargetTag, const _uint2 vResolution, const DXGI_FORMAT, const _color vClear);

	HRESULT															Begin_MultiRenderTaget(const wstring& wstrMultiTargetTag, _uint iCopyResource = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT);
	HRESULT															End_MultiRenderTarget();

	HRESULT															Bind_RenderTarget(const wstring& wstrTargetTag, shared_ptr<class CShader>, const _char* szConstantName);

private:
	_bool															Exist_RenderTarget(const wstring& wstrTargetTag);
	_bool															Exist_MultiRenderTarget(const wstring& wstrMultiTargetTag);

private:
	unordered_map<wstring, shared_ptr<class CRenderTarget>>			m_umapRenderTarget;
	unordered_map<wstring, list<shared_ptr<class CRenderTarget>>>	m_umapMultiRenderTarget;

	ComPtr<ID3D11RenderTargetView>									m_pBackBufferView;
	ComPtr<ID3D11DepthStencilView>									m_pDepthStencilView;

	ComPtr<ID3D11Device>											m_pDevice;
	ComPtr<ID3D11DeviceContext>										m_pContext;

	friend CSingleton<CRenderTarget_Manager>;
};

END
