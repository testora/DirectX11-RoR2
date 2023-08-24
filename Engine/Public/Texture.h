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
	HRESULT										Initialize(const _tchar* szTexturePath, _uint iNumTexture = 1);
	HRESULT										Initialize(const char* szModelPath, aiMaterial*, aiTextureType);

public:
	ComPtr<ID3D11Texture2D>						Get_Texture2D(_uint iTextureIdx = 0) const;
	HRESULT										Set_Texture2D(ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC, _uint iTextureIdx = 0);

	HRESULT										Bind_ShaderResourceView(shared_ptr<class CShader>, aiTextureType, const char* szConstantName, _uint iTextureIdx = 0) const;
	HRESULT										Bind_ShaderResourceViews(shared_ptr<class CShader>, aiTextureType, const char* szConstantName);

private:
	ComPtr<ID3D11ShaderResourceView>			Create_ShaderResourceView(const _tchar* szTexturePath) const;

private:
	vector<ComPtr<ID3D11ShaderResourceView>>	m_vecTexture;

public:
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _tchar* szTexturePath, _uint iNumTextures = 1);
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const char* szModelPath, aiMaterial*, aiTextureType);
	virtual shared_ptr<CComponent>				Clone(any = any()) override;
};

END
