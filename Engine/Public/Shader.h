#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	explicit CShader(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CShader(const CShader&);
	virtual ~CShader() DEFAULT;

public:
	HRESULT								Initialize(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElememts, _uint iNumElements);

public:
	void								Set_Flag(_uint iStatus);

public:
	HRESULT								BeginPass(const _uint iPassIndex);

	HRESULT								Bind_RawValue(const char* pConstantName, const void* pData, _uint iByteSize);
	HRESULT								Bind_Vector(const char* pConstantName, _float4);
	HRESULT								Bind_VectorArray(const char* pConstantName, _float4*, _uint iCount);
	HRESULT								Bind_Matrix(const char* pConstantName, _float4x4);
	HRESULT								Bind_MatrixArray(const char* pConstantName, _float4x4*, _uint iCount);

	HRESULT								Bind_ShaderResourceView(const char* pConstantName, ComPtr<ID3D11ShaderResourceView>);
	HRESULT								Bind_ShaderResourceViews(const char* pConstantName, vector<ComPtr<ID3D11ShaderResourceView>>&);

private:
	ComPtr<ID3DX11Effect>				m_pEffect;
	vector<ComPtr<ID3D11InputLayout>>	m_vecInputLayout;

	_uint 								m_iNumPasses	= 0;

	_uint 								m_iShaderFlag	= 0;

public:
	static shared_ptr<CShader>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElememts, _uint iNumElement);
	virtual shared_ptr<CComponent>		Clone(any = any()) override;
};

END
