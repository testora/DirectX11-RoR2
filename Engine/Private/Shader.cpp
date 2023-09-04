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

HRESULT CShader::Initialize(const wstring& _wstrShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* _pElememts, _uint _iNumElements)
{
#ifdef _DEBUG
	_flags iHLSLFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	_flags iHLSLFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	if (FAILED(D3DX11CompileEffectFromFile(_wstrShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHLSLFlag, 0, m_pDevice.Get(), m_pEffect.ReleaseAndGetAddressOf(), nullptr)))
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

void CShader::Add_Flag(_flags _iStatus)
{
	m_iShaderFlag |= _iStatus;
}

void CShader::Set_Flag(_flags _iStatus)
{
	if (_iStatus)
	{
		m_iShaderFlag = _iStatus;
	}
}

void CShader::Remove_Flag(_flags _iStatus)
{
	_iStatus ? m_iShaderFlag &= ~_iStatus : m_iShaderFlag = 0;
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

	if (FAILED(Bind_Int(SHADER_FLAG, m_iShaderFlag)))
	{
		MSG_RETURN(E_FAIL, "CShader::BeginPass", "Failed to Bind_RawValue");
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

HRESULT CShader::Bind_RawValue(const _char* _szConstantName, const void* _pData, _uint _iByteSize)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_szConstantName);
	if (nullptr == pVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_RawValue", "Failed to GetVariableByName");
	}

	if (FAILED(pVariable->SetRawValue(_pData, 0, _iByteSize)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_RawValue", "Failed to SetRawValue");
	}

	return S_OK;
}

HRESULT CShader::Bind_Int(const _char* _szConstantName, const _int _iValue)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectScalarVariable* pScalarVariable = m_pEffect->GetVariableByName(_szConstantName)->AsScalar();
	if (nullptr == pScalarVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Int", "Failed to GetVariableByName::AsScalar");
	}

	if (FAILED(pScalarVariable->SetInt(_iValue)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Int", "Failed to SetInt");
	}

	return S_OK;
}

HRESULT CShader::Bind_IntArray(const _char* _szConstantName, const _int* _pArray, _uint _iCount)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectScalarVariable* pScalarVariable = m_pEffect->GetVariableByName(_szConstantName)->AsScalar();
	if (nullptr == pScalarVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_IntArray", "Failed to GetVariableByName::AsScalar");
	}

	if (FAILED(pScalarVariable->SetIntArray(_pArray, 0, _iCount)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_IntArray", "Failed to SetIntArray");
	}

	return S_OK;
}

HRESULT CShader::Bind_Float(const _char* _szConstantName, const _float _fValue)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectScalarVariable* pScalarVariable = m_pEffect->GetVariableByName(_szConstantName)->AsScalar();
	if (nullptr == pScalarVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Float", "Failed to GetVariableByName::AsScalar");
	}

	if (FAILED(pScalarVariable->SetFloat(_fValue)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Float", "Failed to SetFloat");
	}

	return S_OK;
}

HRESULT CShader::Bind_FloatArray(const _char* _szConstantName, const _float* _pArray, _uint _iCount)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectScalarVariable* pScalarVariable = m_pEffect->GetVariableByName(_szConstantName)->AsScalar();
	if (nullptr == pScalarVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_FloatArray", "Failed to GetVariableByName::AsScalar");
	}

	if (FAILED(pScalarVariable->SetFloatArray(_pArray, 0, _iCount)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_FloatArray", "Failed to SetFloatArray");
	}

	return S_OK;
}

HRESULT CShader::Bind_Vector(const _char* _szConstantName, const _float4 _vValue)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectVectorVariable* pVectorVariable = m_pEffect->GetVariableByName(_szConstantName)->AsVector();
	if (nullptr == pVectorVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Vector", "Failed to GetVariableByName::AsVector");
	}

	if (FAILED(pVectorVariable->SetFloatVector(reinterpret_cast<const _float*>(&_vValue))))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Vector", "Failed to SetFloatVectorArray");
	}

	return S_OK;
}

HRESULT CShader::Bind_VectorArray(const _char* _szConstantName, const _float4* _pArray, _uint _iCount)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectVectorVariable* pVectorVariable = m_pEffect->GetVariableByName(_szConstantName)->AsVector();
	if (nullptr == pVectorVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_VectorArray", "Failed to GetVariableByName::AsVector");
	}

	if (FAILED(pVectorVariable->SetFloatVectorArray(reinterpret_cast<const _float*>(_pArray), 0, _iCount)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_VectorArray", "Failed to SetFloatVectorArray");
	}

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* _szConstantName, const _float4x4 _mValue)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectMatrixVariable* pMatrixVariable = m_pEffect->GetVariableByName(_szConstantName)->AsMatrix();
	if (nullptr == pMatrixVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Matrix", "Failed to GetVariableByName::AsMatrix");
	}

	if (FAILED(pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(&_mValue))))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_Matrix", "Failed to SetMatrix");
	}

	return S_OK;
}

HRESULT CShader::Bind_MatrixArray(const _char* _szConstantName, const _float4x4* _pArray, _uint _iCount)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectMatrixVariable* pMatrixVariable = m_pEffect->GetVariableByName(_szConstantName)->AsMatrix();
	if (nullptr == pMatrixVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_MatrixArray", "Failed to GetVariableByName::AsMatrix");
	}

	if (FAILED(pMatrixVariable->SetMatrixArray(reinterpret_cast<const _float*>(_pArray), 0, _iCount)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_MatrixArray", "Failed to SetMatrixArray");
	}

	return S_OK;
}

HRESULT CShader::Bind_ShaderResourceView(const _char* _szConstantName, _In_ ComPtr<ID3D11ShaderResourceView> _pShaderResourceView)
{
	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = m_pEffect->GetVariableByName(_szConstantName)->AsShaderResource();
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

HRESULT CShader::Bind_ShaderResourceViews(const _char* _szConstantName, _In_ ID3D11ShaderResourceView** _ppcShaderResourceViews, _uint _iOffset, _uint _iCount)
{
	if (0 == _iCount)
	{
		return S_FALSE;
	}

	if (nullptr == m_pEffect)
	{
		return E_FAIL;
	}

	ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = m_pEffect->GetVariableByName(_szConstantName)->AsShaderResource();
	if (nullptr == pShaderResourceVariable)
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_ShaderResourceViews", "Failed to GetVariableByName");
	}

	if (FAILED(pShaderResourceVariable->SetResourceArray(_ppcShaderResourceViews, _iOffset, _iCount)))
	{
		MSG_RETURN(E_FAIL, "CShader::Bind_ShaderResourceViews", "Failed to SetResource");
	}

	return S_OK;
}

shared_ptr<CShader> CShader::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* _pElememts, _uint _iNumElement)
{
	shared_ptr<CShader> pInstance = make_private_shared(CShader, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_wstrShaderFilePath, _pElememts, _iNumElement)))
	{
		MSG_RETURN(nullptr, "CShader::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CShader::Clone(any)
{
	return shared_from_this();
}
