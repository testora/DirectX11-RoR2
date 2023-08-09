#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent, public std::enable_shared_from_this<CTexture>
{
private:
	explicit CTexture(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CTexture(const CTexture&);
	virtual ~CTexture() DEFAULT;

public:
	HRESULT										Initialize(const _tchar* pTexturePath, _uint iNumTexture = 1);

public:
	ComPtr<ID3D11Texture2D>						Get_Texture2D(_uint iTextureIdx = 0) const;
	HRESULT										Set_Texture2D(ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC, _uint iTextureIdx = 0);

	HRESULT										Bind_ShaderResourceView(shared_ptr<class CShader>, const char* pConstantName, _uint iTextureIdx = 0) const;
	HRESULT										Bind_ShaderResourceViews(shared_ptr<class CShader>, const char* pConstantName);

private:
	vector<ComPtr<ID3D11ShaderResourceView>>	m_vecTexture;

public:
	static shared_ptr<CTexture>					Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _tchar* pTexturePath, _uint iNumTextures = 1);
	virtual shared_ptr<CComponent>				Clone(any = any()) override;
};

END
