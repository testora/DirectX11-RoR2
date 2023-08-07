#include "EnginePCH.h"
#include "Shader.h"

CShader::CShader(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::SHADER)
{
}

CShader::CShader(const CShader& _rhs)
	: CComponent		(_rhs)
	, m_pEffect			(_rhs.m_pEffect)
	, m_vecInputLayout	(_rhs.m_vecInputLayout)
	, m_iNumPasses		(_rhs.m_iNumPasses)
{
}

HRESULT CShader::Initialize(const _tchar* _pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* _pElememts, _uint _iNumElements)
{
#ifdef _DEBUG
	_uint iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	_uint iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	if (FAILED(D3DX11CompileEffectFromFile(_pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, m_pDevice.Get(), m_pEffect.ReleaseAndGetAddressOf(), nullptr)))
	{
		MSG_RETURN(E_FAIL, "CShader::Initialize", "Failed to D3DX11CompileEffectFromFile");
	}

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
	{
		MSG_RETURN(E_FAIL, "CShader::Initialize", "Failed to GetTechniqueByIndex");
	}

	D3DX11_TECHNIQUE_DESC tTechniqueDesc{};
	if (FAILED(pTechnique->GetDesc(&tTechniqueDesc)))
	{
		MSG_RETURN(E_FAIL, "CShader::Initialize", "Failed to GetDesc");
	}

	m_iNumPasses = tTechniqueDesc.Passes;
	m_vecInputLayout.reserve(m_iNumPasses);

	for (_uint i = 0; i < m_iNumPasses; ++i)
	{
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == pPass)
		{
			MSG_BREAK("CShader::Initialize", "Failed to GetPassByIndex");
		}

		D3DX11_PASS_DESC tPassDesc{};
		if (FAILED(pPass->GetDesc(&tPassDesc)))
		{
			MSG_BREAK("CShader::Initialize", "Failed to GetDesc");
		}

		ComPtr<ID3D11InputLayout> pInputLayout;
		if (FAILED(m_pDevice->CreateInputLayout(_pElememts, _iNumElements, tPassDesc.pIAInputSignature, tPassDesc.IAInputSignatureSize, pInputLayout.ReleaseAndGetAddressOf())))
		{
			MSG_BREAK("CShader::Initialize", "Failed to CreateInputLayout");
		}

		m_vecInputLayout.emplace_back(std::move(pInputLayout));
	}

	return S_OK;
}

HRESULT CShader::BeginPass(const _uint _iPassIndex)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	if (m_iNumPasses <= _iPassIndex)
	{
		MSG_RETURN(E_FAIL, "CShader::BeginPass", "Invalid Range");
	}

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
	{
		MSG_RETURN(E_FAIL, "CShader::BeginPass", "Failed to GetTechniqueByIndex");
	}

	if(FAILED(pTechnique->GetPassByIndex(_iPassIndex)->Apply(0, m_pContext.Get())))
	{
		MSG_RETURN(E_FAIL, "CShader::BeginPass", "Failed to Apply");
	}

	m_pContext->IASetInputLayout(m_vecInputLayout[_iPassIndex].Get());

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const char* _pConstantName, _float4x4 _mMatrix)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectMatrixVariable* pMatrixVariable = m_pEffect->GetVariableByName(_pConstantName)->AsMatrix();
	if (nullptr == pMatrixVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Matrix", "Failed to GetVariableByName");
	}

	if (FAILED(pMatrixVariable->SetMatrix(reinterpret_cast<_float*>(&_mMatrix))))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Matrix", "Failed to SetMatrix");
	}

	return S_OK;
}

HRESULT CShader::Bind_ShaderResourceView(const char* _pConstantName, ComPtr<ID3D11ShaderResourceView> _pShaderResourceView)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = m_pEffect->GetVariableByName(_pConstantName)->AsShaderResource();
	if (nullptr == pShaderResourceVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_ShaderResourceView", "Failed to GetVariableByName");
	}

	if (FAILED(pShaderResourceVariable->SetResource(_pShaderResourceView.Get())))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_ShaderResourceView", "Failed to SetResource");
	}
	
	return S_OK;
}

HRESULT CShader::Bind_ShaderResourceViews(const char* _pConstantName, vector<ComPtr<ID3D11ShaderResourceView>>& _vecShaderResourceView)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = m_pEffect->GetVariableByName(_pConstantName)->AsShaderResource();
	if (nullptr == pShaderResourceVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_ShaderResourceView", "Failed to GetVariableByName");
	}

	auto raw = Function::ConvertToRawPtrVector(_vecShaderResourceView);
	if (FAILED(pShaderResourceVariable->SetResourceArray(raw.data(), 0, static_cast<_uint>(_vecShaderResourceView.size()))))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_ShaderResourceView", "Failed to SetResource");
	}

	return S_OK;
}

shared_ptr<CShader> CShader::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const _tchar* _pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* _pElememts, _uint _iNumElement)
{
	shared_ptr<CShader> pInstance = make_private_shared(CShader, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pShaderFilePath, _pElememts, _iNumElement)))
	{
		MSG_RETURN(nullptr, "CShader::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CShader::Clone(any _arg)
{
	return shared_from_this();
}
