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

HRESULT CTexture::Initialize(const wstring& _strTexturePath, _uint _iNumTexture)
{
	HRESULT hr = S_OK;

	m_vecTexture.reserve(_iNumTexture);

	for (size_t i = 0; i < _iNumTexture; ++i)
	{
		_wchar szFullPath[MAX_PATH] = TEXT("");
		wsprintf(szFullPath, _strTexturePath.c_str(), i);

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

HRESULT CTexture::Initialize(const wstring& _strModelPath, aiMaterial* _pAIMaterial, aiTextureType _eAITextureType)
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

		_char	szDrive[MAX_PATH]		= "";
		_char	szDirectory[MAX_PATH]	= "";
		_char	szFileName[MAX_PATH]	= "";
		_char	szExt[MAX_PATH]			= "";
		_char	szFullPath[MAX_PATH]	= "";
		_wchar	szFinalPath[MAX_PATH]	= TEXT("");
	
		_splitpath_s(Function::ToString(_strModelPath).c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
		_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
	
		strcpy_s(szFullPath, szDrive);
		strcat_s(szFullPath, szDirectory);
		strcat_s(szFullPath, szFileName);
		strcat_s(szFullPath, szExt);

		ComPtr<ID3D11ShaderResourceView> pShaderResourceView = Create_ShaderResourceView(Function::ToWString(szFullPath));
		if (nullptr == pShaderResourceView)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CTexture::Initialize", "Failed to Create_ShaderResourceView");
		}

		m_vecTexture.emplace_back(pShaderResourceView);
#ifdef _DEBUG
#if ACTIVATE_IMGUI
		m_vecTextureFileName.emplace_back(Function::ToWString(szFileName) + Function::ToWString(szExt));
#endif
#endif
	}

	return hr;
}

#ifdef _DEBUG
#if ACTIVATE_IMGUI
ComPtr<ID3D11ShaderResourceView> CTexture::Get_ShaderResourceView(_uint _iTextureIdx) const
{
	if (m_vecTexture.size() <= _iTextureIdx)
	{
		MSG_RETURN(nullptr, "CTexture::Get_ShaderResourceView", "Invalid Range");
	}

	return m_vecTexture[_iTextureIdx];
}
#endif
#endif

ComPtr<ID3D11Texture2D> CTexture::Get_Texture2D(_uint _iTextureIdx) const
{
	if (m_vecTexture.size() <= _iTextureIdx)
	{
		MSG_RETURN(nullptr, "CTexture::Get_Texture2D", "Invalid Range");
	}

	ComPtr<ID3D11Resource> pResource;
	m_vecTexture[_iTextureIdx]->GetResource(pResource.GetAddressOf());

	ComPtr<ID3D11Texture2D> pTexture2D;
	if (FAILED(pResource.As(&pTexture2D)))
	{
		MSG_RETURN(nullptr, "CTexture::Get_Texture2D", "Failed to As");
	}

	return pTexture2D;
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

#ifdef _DEBUG
#if ACTIVATE_IMGUI
HRESULT CTexture::Push_ShaderResourceView(const wstring& _strTexturePath)
{
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView = Create_ShaderResourceView(_strTexturePath);
	if (nullptr == pShaderResourceView)
	{
		MSG_RETURN(E_FAIL, "CTexture::Push_ShaderResourceView", "Failed to Create_ShaderResourceView");
	}

	m_vecTexture.emplace_back(pShaderResourceView);

	_wchar szFileName[MAX_PATH]	= TEXT("");
	_wchar szExt[MAX_PATH]		= TEXT("");
	_wsplitpath_s(_strTexturePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	m_vecTextureFileName.emplace_back(wstring(szFileName) + wstring(szExt));

	return S_OK;
}

HRESULT CTexture::Remove_ShaderResourceView(_uint _iTextureIdx)
{
	if (m_vecTexture.size() <= _iTextureIdx)
	{
		MSG_RETURN(E_FAIL, "CTexture::Remove_ShaderResourceView", "Invalid Range");
	}

	m_vecTexture.erase(m_vecTexture.begin() + _iTextureIdx);
	m_vecTextureFileName.erase(m_vecTextureFileName.begin() + _iTextureIdx);

	return S_OK;
}
#endif
#endif

HRESULT CTexture::Bind_ShaderResourceView(shared_ptr<CShader> _pShader, aiTextureType _eAiType, const _char* _szConstantName, _uint _iTextureIdx) const
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

HRESULT CTexture::Bind_ShaderResourceViews(shared_ptr<CShader> _pShader, aiTextureType _eAiType, const _char* _szConstantName)
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

ComPtr<ID3D11ShaderResourceView> CTexture::Create_ShaderResourceView(const wstring& _strTexturePath) const
{
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	_wchar szExt[MAX_PATH]		= TEXT("");
	_wsplitpath_s(_strTexturePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

	assert(lstrcmp(szExt, TEXT(".tga")));

	if (false == lstrcmp(szExt, TEXT(".dds")))
	{
		if (FAILED(CreateDDSTextureFromFile(m_pDevice.Get(), _strTexturePath.c_str(), nullptr, pShaderResourceView.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CTexture::Initialize", "Failed to CreateDDSTextureFromFile");
		}
	}
	else
	{
		if (FAILED(CreateWICTextureFromFile(m_pDevice.Get(), _strTexturePath.c_str(), nullptr, pShaderResourceView.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CTexture::Initialize", "Failed to CreateWICTextureFromFile");
		}
	}

	return pShaderResourceView;
}

shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strTexturePath, _uint _iNumTextures)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_strTexturePath, _iNumTextures)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strModelPath, aiMaterial* _pAIMaterial, aiTextureType _eAITextureType)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_strModelPath, _pAIMaterial, _eAITextureType)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CTexture::Clone(any)
{
	return shared_from_this();
}
