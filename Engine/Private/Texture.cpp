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

HRESULT CTexture::Initialize(const _tchar* _szTexturePath, _uint _iNumTexture)
{
	HRESULT hr = S_OK;

	m_vecTexture.reserve(_iNumTexture);

	for (size_t i = 0; i < _iNumTexture; ++i)
	{
		_tchar szFullPath[MAX_PATH]	= TEXT("");
		wsprintf(szFullPath, _szTexturePath, i);

		ComPtr<ID3D11ShaderResourceView> pShaderResourceView = Create_ShaderResourceView(szFullPath);
		if (nullptr == pShaderResourceView)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CTexture::Initialize", "Failed to Create_ShaderResourceView");
		}

		m_vecTexture.emplace_back(pShaderResourceView);
	}

	return hr;
}

HRESULT CTexture::Initialize(const char* _szModelPath, aiMaterial* _pAIMaterial, aiTextureType _eAITextureType)
{
	HRESULT hr = S_OK;

	m_vecTexture.reserve(_pAIMaterial->GetTextureCount(_eAITextureType));

	for (_uint i = 0; i < g_iMaxTextures; ++i)
	{
		aiString strPath;

		if (FAILED(_pAIMaterial->GetTexture(_eAITextureType, i, &strPath)))
		{
			continue;
		}

		char	szDrive[MAX_PATH]		= "";
		char	szDirectory[MAX_PATH]	= "";
		char	szFileName[MAX_PATH]	= "";
		char	szExt[MAX_PATH]			= "";
		char	szFullPath[MAX_PATH]	= "";
		_tchar	szFinalPath[MAX_PATH]	= L"";
	
		_splitpath_s(_szModelPath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
		_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
	
		strcpy_s(szFullPath, szDrive);
		strcat_s(szFullPath, szDirectory);
		strcat_s(szFullPath, szFileName);
		strcat_s(szFullPath, szExt);
	
		MultiByteToWideChar(CP_ACP, 0, szFullPath, static_cast<int>(strlen(szFullPath)), szFinalPath, MAX_PATH);

		ComPtr<ID3D11ShaderResourceView> pShaderResourceView = Create_ShaderResourceView(szFinalPath);
		if (nullptr == pShaderResourceView)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CTexture::Initialize", "Failed to Create_ShaderResourceView");
		}

		m_vecTexture.emplace_back(pShaderResourceView);
	}

	return hr;
}

ComPtr<ID3D11Texture2D> CTexture::Get_Texture2D(_uint _iTextureIdx) const
{
	if (m_vecTexture.size() <= _iTextureIdx)
	{
		MSG_RETURN(nullptr, "CTexture::Get_Texture2D", "Invalid Range");
	}

	ComPtr<ID3D11Resource> pResource;
	m_vecTexture[_iTextureIdx]->GetResource(pResource.GetAddressOf());

	ComPtr<ID3D11Texture2D> pTexture2D;
	if (SUCCEEDED(pResource.As(&pTexture2D)))
	{
		return pTexture2D;
	}

	MSG_RETURN(nullptr, "CTexture::Get_Texture2D", "Failed to As");
}

HRESULT CTexture::Set_Texture2D(ComPtr<ID3D11Texture2D> _pTexture, D3D11_TEXTURE2D_DESC _tDesc, _uint _iTextureIdx)
{
	if (m_vecTexture.size() <= _iTextureIdx)
	{
		MSG_RETURN(E_FAIL, "CTexture::Set_Texture2D", "Invalid Range");
	}

	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVc{};
	tSRVc.Format					= _tDesc.Format;
	tSRVc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	tSRVc.Texture2D.MipLevels		= _tDesc.MipLevels;
	tSRVc.Texture2D.MostDetailedMip	= 0;

	if (FAILED(m_pDevice->CreateShaderResourceView(_pTexture.Get(), &tSRVc, pShaderResourceView.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CTexture::Set_Texture2D", "Failed to CreateShaderResourceView");
	}

	m_vecTexture[_iTextureIdx] = pShaderResourceView;

	return S_OK;
}

HRESULT CTexture::Bind_ShaderResourceView(shared_ptr<CShader> _pShader, aiTextureType _eAiType, const char* _szConstantName, _uint _iTextureIdx) const
{
	if (m_vecTexture.size() <= _iTextureIdx)
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Invalid Range");
	}

	if (FAILED(_pShader->Bind_ShaderResourceView(_szConstantName, m_vecTexture[_iTextureIdx])))
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	switch (_eAiType)
	{
	case aiTextureType_DIFFUSE:
		_pShader->Set_Flag(SHADER_FLAG_TEXDIFFUSE);
		break;

	case aiTextureType_NORMALS:
		_pShader->Set_Flag(SHADER_FLAG_TEXNORMAL);
		break;

	default:
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Invalid aiTextureType");
	}

	return S_OK;
}

HRESULT CTexture::Bind_ShaderResourceViews(shared_ptr<CShader> _pShader, aiTextureType _eAiType, const char* _szConstantName)
{
	if (FAILED(_pShader->Bind_ShaderResourceViews(_szConstantName, m_vecTexture)))
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	switch (_eAiType)
	{
	case aiTextureType_DIFFUSE:
		_pShader->Set_Flag(SHADER_FLAG_TEXDIFFUSE);
		break;

	case aiTextureType_NORMALS:
		_pShader->Set_Flag(SHADER_FLAG_TEXNORMAL);
		break;

	default:
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceViews", "Invalid aiTextureType");
	}

	return S_OK;
}

ComPtr<ID3D11ShaderResourceView> CTexture::Create_ShaderResourceView(const _tchar* _szTexturePath) const
{
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	_tchar szExt[MAX_PATH]		= TEXT("");
	_wsplitpath_s(_szTexturePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

	assert(lstrcmp(szExt, TEXT(".tga")));

	if (false == lstrcmp(szExt, TEXT(".dds")))
	{
		if (FAILED(CreateDDSTextureFromFile(m_pDevice.Get(), _szTexturePath, nullptr, pShaderResourceView.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CTexture::Initialize", "Failed to CreateDDSTextureFromFile");
		}
	}
	else
	{
		if (FAILED(CreateWICTextureFromFile(m_pDevice.Get(), _szTexturePath, nullptr, pShaderResourceView.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CTexture::Initialize", "Failed to CreateWICTextureFromFile");
		}
	}

	return pShaderResourceView;
}

shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const _tchar* _szTexturePath, _uint _iNumTextures)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_szTexturePath, _iNumTextures)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const char* _szModelPath, aiMaterial* _pAIMaterial, aiTextureType _eAITextureType)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_szModelPath, _pAIMaterial, _eAITextureType)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CTexture::Clone(any)
{
	return shared_from_this();
}
