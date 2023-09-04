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
	HRESULT								Initialize(const wstring& wstrShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElememts, _uint iNumElements);

public:
	void								Add_Flag(_flags iStatus);
	void								Set_Flag(_flags iStatus);
	void								Remove_Flag(_flags iStatus = 0);

public:
	HRESULT								BeginPass(const _uint iPassIndex);

	HRESULT								Bind_RawValue(const _char* szConstantName, const void* pData, _uint iByteSize);
	HRESULT								Bind_Int(const _char* szConstantName, const _int);
	HRESULT								Bind_IntArray(const _char* szConstantName, const _int*, _uint iCount);
	HRESULT								Bind_Float(const _char* szConstantName, const _float);
	HRESULT								Bind_FloatArray(const _char* szConstantName, const _float*, _uint iCount);
	HRESULT								Bind_Vector(const _char* szConstantName, const _float4);
	HRESULT								Bind_VectorArray(const _char* szConstantName, const _float4*, _uint iCount);
	HRESULT								Bind_Matrix(const _char* szConstantName, const _float4x4);
	HRESULT								Bind_MatrixArray(const _char* szConstantName, const _float4x4*, _uint iCount);

	HRESULT								Bind_ShaderResourceView(const _char* szConstantName, _In_ ComPtr<ID3D11ShaderResourceView>);
	HRESULT								Bind_ShaderResourceViews(const _char* szConstantName, _In_ ID3D11ShaderResourceView**, _uint iOffset = 0, _uint iCount = 1);

private:
	ComPtr<ID3DX11Effect>				m_pEffect;
	vector<ComPtr<ID3D11InputLayout>>	m_vecInputLayout;

	_uint 								m_iNumPasses	= 0;

	_flags 								m_iShaderFlag	= 0;

public:
	static shared_ptr<CShader>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElememts, _uint iNumElement);
	virtual shared_ptr<CComponent>		Clone(any = g_aNull) override;
};

END
