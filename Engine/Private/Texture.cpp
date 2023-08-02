#include "EnginePCH.h"
#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::TEXTURE)
{
}

CTexture::CTexture(const CTexture& _rhs)
	: CComponent	(_rhs)
	, m_vecTexture	(_rhs.m_vecTexture)
{
}

HRESULT CTexture::Initialize(const _tchar* _pTexturePath, _uint _iNumTexture)
{
	m_vecTexture.reserve(_iNumTexture);

	for (size_t i = 0; i < _iNumTexture; ++i)
	{
		ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

		_tchar szFullPath[MAX_PATH]	= TEXT("");
		_tchar szExt[MAX_PATH]		= TEXT("");

		wsprintf(szFullPath, _pTexturePath, i);
		_wsplitpath_s(szFullPath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		assert(lstrcmp(szExt, TEXT(".tga")));

		if (false == lstrcmp(szExt, TEXT(".dds")))
		{
			if (FAILED(CreateDDSTextureFromFile(m_pDevice.Get(), szFullPath, nullptr, pShaderResourceView.GetAddressOf())))
			{
				MSG_RETURN(E_FAIL, "CTexture::Initialize", "Failed to CreateDDSTextureFromFile");
			}
		}
		else
		{
			if (FAILED(CreateWICTextureFromFile(m_pDevice.Get(), szFullPath, nullptr, pShaderResourceView.GetAddressOf())))
			{
				MSG_RETURN(E_FAIL, "CTexture::Initialize", "Failed to CreateWICTextureFromFile");
			}
		}

		m_vecTexture.emplace_back(std::move(pShaderResourceView));
	}

	return S_OK;
}

HRESULT CTexture::Bind_ShaderResourceView(shared_ptr<CShader> _pShader, const char* _pConstantName, _uint _iTextureIdx)
{
	if (m_vecTexture.size() <= _iTextureIdx)
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Invalid Range");
	}

	if (FAILED(_pShader->Bind_ShaderResourceView(_pConstantName, m_vecTexture[_iTextureIdx])))
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	return S_OK;
}

HRESULT CTexture::Bind_ShaderResourceViews(shared_ptr<CShader> _pShader, const char* _pConstantName)
{
	if (FAILED(_pShader->Bind_ShaderResourceViews(_pConstantName, m_vecTexture)))
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	return S_OK;
}

shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const _tchar* _pTexturePath, _uint _iNumTextures)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pTexturePath, _iNumTextures)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CTexture::Clone(std::any)
{
	return shared_from_this();
}
