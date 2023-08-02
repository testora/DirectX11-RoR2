#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent, public std::enable_shared_from_this<CShader>
{
private:
	explicit CShader(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CShader(const CShader&);
	virtual ~CShader() DEFAULT;

public:
	HRESULT								Initialize(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElememts, _uint iNumElements);

public:
	HRESULT								BeginPass(const _uint iPassIndex);
	HRESULT								Bind_Matrix(const char* pConstantName, _float4x4);
	HRESULT								Bind_ShaderResourceView(const char* pConstantName, ComPtr<ID3D11ShaderResourceView> pShaderResourceView);
	HRESULT								Bind_ShaderResourceViews(const char* pConstantName, vector<ComPtr<ID3D11ShaderResourceView>>& vecShaderResourceView);

private:
	ComPtr<ID3DX11Effect>				m_pEffect;
	vector<ComPtr<ID3D11InputLayout>>	m_vecInputLayout;

	_uint 								m_iNumPasses	= 0;

public:
	static shared_ptr<CShader>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElememts, _uint iNumElement);
	virtual shared_ptr<CComponent>		Clone(std::any = nullptr) override;
};

END
