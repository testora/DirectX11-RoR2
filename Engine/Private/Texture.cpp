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

HRESULT CTexture::Initialize(_uint _iNumTextures)
{
	m_vecTexture.reserve(_iNumTextures);

	return S_OK;
}

HRESULT CTexture::Initialize(const wstring& _wstrTexturePath, _uint _iNumTextures)
{
	HRESULT hr = S_OK;

	m_vecTexture.reserve(_iNumTextures);

	for (size_t i = 0; i < _iNumTextures; ++i)
	{
		_wchar szFullPath[MAX_PATH] = TEXT("");
		wsprintf(szFullPath, _wstrTexturePath.c_str(), i);

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

#if ACTIVATE_TOOL
HRESULT CTexture::Initialize(const wstring& _wstrModelPath, aiMaterial* _pAIMaterial, aiTextureType _eAITextureType)
{
	HRESULT hr = S_OK;

	m_vecTexture.reserve(_pAIMaterial->GetTextureCount(_eAITextureType));

	for (_uint i = 0; i < g_iMaxTextures; ++i)
	{
		aiString aiStrPath;
		if (FAILED(_pAIMaterial->GetTexture(_eAITextureType, i, &aiStrPath)))
		{
			continue;
		}

		wstring	wstrDrive, wstrDirectory;
		string	strFileName, strExt;
		Function::SplitPath(_wstrModelPath, &wstrDrive, &wstrDirectory, nullptr, nullptr);
		Function::SplitPath(aiStrPath.C_Str(), nullptr, nullptr, &strFileName, &strExt);

		wstring wstrTexturePath = wstrDrive + wstrDirectory + Function::ToWString(strFileName + strExt);

		ComPtr<ID3D11ShaderResourceView> pShaderResourceView = Create_ShaderResourceView(wstrTexturePath);
		if (nullptr == pShaderResourceView)
		{
			hr = E_FAIL;
			MSG_CONTINUE("CTexture::Initialize", "Failed to Create_ShaderResourceView");
		}

		m_vecTexture.emplace_back(pShaderResourceView);
		m_vecTexturePath.emplace_back(wstrTexturePath);
	}

	return hr;
}
#endif

#if ACTIVATE_TOOL
ComPtr<ID3D11ShaderResourceView> CTexture::Get_ShaderResourceView(_uint _iTextureIndex) const
{
	if (m_vecTexture.size() <= _iTextureIndex)
	{
		MSG_RETURN(nullptr, "CTexture::Get_ShaderResourceView", "Invalid Range");
	}

	return m_vecTexture[_iTextureIndex];
}
#endif

ComPtr<ID3D11Texture2D> CTexture::Get_Texture2D(_uint _iTextureIndex) const
{
	if (m_vecTexture.size() <= _iTextureIndex)
	{
		MSG_RETURN(nullptr, "CTexture::Get_Texture2D", "Invalid Range");
	}

	ComPtr<ID3D11Resource> pResource;
	m_vecTexture[_iTextureIndex]->GetResource(pResource.GetAddressOf());

	ComPtr<ID3D11Texture2D> pTexture2D;
	if (FAILED(pResource.As(&pTexture2D)))
	{
		MSG_RETURN(nullptr, "CTexture::Get_Texture2D", "Failed to As");
	}

	return pTexture2D;
}

HRESULT CTexture::Set_Texture2D(ComPtr<ID3D11Texture2D> _pTexture, D3D11_TEXTURE2D_DESC _tDesc, _uint _iTextureIndex)
{
	if (m_vecTexture.size() <= _iTextureIndex)
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

	m_vecTexture[_iTextureIndex] = pShaderResourceView;

	return S_OK;
}

HRESULT CTexture::Push_ShaderResourceView(const wstring& _wstrFullPath)
{
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView = Create_ShaderResourceView(_wstrFullPath);
	if (nullptr == pShaderResourceView)
	{
		MSG_RETURN(E_FAIL, "CTexture::Push_ShaderResourceView", "Failed to Create_ShaderResourceView");
	}

	m_vecTexture.emplace_back(pShaderResourceView);
#if ACTIVATE_TOOL
	m_vecTexturePath.emplace_back(_wstrFullPath);
#endif

	return S_OK;
}

#if ACTIVATE_TOOL
HRESULT CTexture::Remove_ShaderResourceView(_uint _iTextureIndex)
{
	if (m_vecTexture.size() <= _iTextureIndex)
	{
		MSG_RETURN(E_FAIL, "CTexture::Remove_ShaderResourceView", "Invalid Range");
	}

	m_vecTexture.erase(m_vecTexture.begin() + _iTextureIndex);
	m_vecTexturePath.erase(m_vecTexturePath.begin() + _iTextureIndex);

	return S_OK;
}
#endif

HRESULT CTexture::Bind_ShaderResourceView(shared_ptr<CShader> _pShader, aiTextureType _eAiType, const _char* _szConstantName, _uint _iTextureIndex) const
{
	if (m_vecTexture.size() <= _iTextureIndex)
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Invalid Range");
	}

	if (FAILED(_pShader->Bind_ShaderResourceView(_szConstantName, m_vecTexture[_iTextureIndex])))
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	switch (_eAiType)
	{
	case aiTextureType_DIFFUSE:
		_pShader->Add_Flag(SHADER_FLAG_TEXDIFFUSE);
		break;

	case aiTextureType_NORMALS:
		_pShader->Add_Flag(SHADER_FLAG_TEXNORMAL);
		break;

	default:
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Invalid aiTextureType");
	}

	return S_OK;
}

HRESULT CTexture::Bind_ShaderResourceViews(shared_ptr<CShader> _pShader, aiTextureType _eAiType, const _char* _szConstantName)
{
	if (FAILED(_pShader->Bind_ShaderResourceViews(_szConstantName, Function::ConvertToRawPtrVector(m_vecTexture).data(), 0, static_cast<_uint>(m_vecTexture.size()))))
	{
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceView", "Failed to Bind_ShaderResourceView");
	}

	switch (_eAiType)
	{
	case aiTextureType_DIFFUSE:
		_pShader->Add_Flag(SHADER_FLAG_TEXDIFFUSE);
		break;

	case aiTextureType_NORMALS:
		_pShader->Add_Flag(SHADER_FLAG_TEXNORMAL);
		break;

	default:
		MSG_RETURN(E_FAIL, "CTexture::Bind_ShaderResourceViews", "Invalid aiTextureType");
	}

	return S_OK;
}

#if ACTIVATE_TOOL
_bool CTexture::Swap_ShaderResourceView(_uint _iTextureIndex1, _uint _iTextureIndex2)
{
	if (!Function::InRange(_iTextureIndex1, 0u, static_cast<_uint>(m_vecTexture.size()))
	||	!Function::InRange(_iTextureIndex2, 0u, static_cast<_uint>(m_vecTexture.size())))
	{
		return false;
	}

	Function::Swap(m_vecTexture[_iTextureIndex1], m_vecTexture[_iTextureIndex2]);
	Function::Swap(m_vecTexturePath[_iTextureIndex1], m_vecTexturePath[_iTextureIndex2]);

	return true;
}
#endif

ComPtr<ID3D11ShaderResourceView> CTexture::Create_ShaderResourceView(const wstring& _wstrFullPath) const
{
	ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

	wstring strExt(_wstrFullPath.substr(_wstrFullPath.rfind(L'.')));

	assert(strExt != TEXT(".tga"));

	if (TEXT(".dds") == strExt)
	{
		if (FAILED(CreateDDSTextureFromFile(m_pDevice.Get(), _wstrFullPath.c_str(), nullptr, pShaderResourceView.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CTexture::Initialize", "Failed to CreateDDSTextureFromFile");
		}
	}
	else
	{
		if (FAILED(CreateWICTextureFromFile(m_pDevice.Get(), _wstrFullPath.c_str(), nullptr, pShaderResourceView.GetAddressOf())))
		{
			MSG_RETURN(nullptr, "CTexture::Initialize", "Failed to CreateWICTextureFromFile");
		}
	}

	return pShaderResourceView;
}

shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, _uint _iNumTextures)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_iNumTextures)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrTexturePath, _uint _iNumTextures)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_wstrTexturePath, _iNumTextures)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}

#if ACTIVATE_TOOL
shared_ptr<CTexture> CTexture::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrModelPath, aiMaterial* _pAIMaterial, aiTextureType _eAITextureType)
{
	shared_ptr<CTexture> pInstance = make_private_shared(CTexture, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_wstrModelPath, _pAIMaterial, _eAITextureType)))
	{
		MSG_RETURN(nullptr, "CTexture::Create", "Failed to Initialize");
	}

	return pInstance;
}
#endif

shared_ptr<CComponent> CTexture::Clone(any)
{
	return shared_from_componenet();
}

#if ACTIVATE_TOOL
void CTexture::Export(std::ofstream& _outFile)
{
	const size_t nNumTextures(m_vecTexture.size());
	_outFile.write(reinterpret_cast<const _byte*>(&nNumTextures), sizeof(size_t));

	for (auto& wstrPath : m_vecTexturePath)
	{
		const size_t nPathLength(wstrPath.length());

		_outFile.write(reinterpret_cast<const _byte*>(&nPathLength),		sizeof(size_t));
		_outFile.write(reinterpret_cast<const _byte*>(wstrPath.c_str()),	sizeof(_wchar)	* nPathLength);
	}
}
#endif
