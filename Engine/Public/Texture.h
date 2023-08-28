#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	explicit CTexture(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTexture(const CTexture&);
	virtual ~CTexture() DEFAULT;

public:
	HRESULT										Initialize(const wstring& strTexturePath, _uint iNumTexture = 1);
	HRESULT										Initialize(const wstring& strModelPath, aiMaterial*, aiTextureType);

public:
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	ComPtr<ID3D11ShaderResourceView>			Get_ShaderResourceView(_uint iTextureIdx = 0) const;
	vector<wstring>								Get_TextureFileName() const	{ return m_vecTextureFileName; }
#endif
#endif
	ComPtr<ID3D11Texture2D>						Get_Texture2D(_uint iTextureIdx = 0) const;
	HRESULT										Set_Texture2D(ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC, _uint iTextureIdx = 0);

public:
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	HRESULT										Push_ShaderResourceView(const wstring& strTexturePath);
	HRESULT										Remove_ShaderResourceView(_uint iTextureIdx);
#endif
#endif
	HRESULT										Bind_ShaderResourceView(shared_ptr<class CShader>, aiTextureType, const _char* szConstantName, _uint iTextureIdx = 0) const;
	HRESULT										Bind_ShaderResourceViews(shared_ptr<class CShader>, aiTextureType, const _char* szConstantName);

private:
	ComPtr<ID3D11ShaderResourceView>			Create_ShaderResourceView(const wstring& strTexturePath) const;

private:
	vector<ComPtr<ID3D11ShaderResourceView>>	m_vecTexture;
#ifdef _DEBUG
#if ACTIVATE_IMGUI
	vector<wstring>								m_vecTextureFileName;
#endif
#endif

public:
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& strTexturePath, _uint iNumTextures = 1);
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& strModelPath, aiMaterial*, aiTextureType);
	virtual shared_ptr<CComponent>				Clone(any = any()) override;
};

END
