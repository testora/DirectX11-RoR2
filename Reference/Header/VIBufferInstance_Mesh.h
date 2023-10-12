#pragma once
#include "Engine_Define.h"
#include "VIBufferInstance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstance_Mesh final : public CVIBufferInstance
{
private:
	explicit CVIBufferInstance_Mesh(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBufferInstance_Mesh(const CVIBufferInstance_Mesh&);
	virtual ~CVIBufferInstance_Mesh() DEFAULT;

public:
	HRESULT										Initialize_Prototype(const wstring& wstrPath, _uint iNumInstance);
	HRESULT										Initialize(any = g_aNull);

private:
	_char										m_szName[MAX_PATH]	= "";
	_uint										m_iMaterialIndex	= 0;

public:
	static shared_ptr<CVIBufferInstance_Mesh>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrPath, _uint iNumInstance);
	virtual shared_ptr<CComponent>				Clone(any = g_aNull) override;
};

END
