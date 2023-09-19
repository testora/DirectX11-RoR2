#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cell final : public CVIBuffer
{
private:
	explicit CVIBuffer_Cell(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual ~CVIBuffer_Cell() DEFAULT;

public:
	HRESULT								Initialize(POLYGON);

public:
	static shared_ptr<CVIBuffer_Cell>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, POLYGON);
	virtual shared_ptr<CComponent>		Clone(any = g_aNull) override;
};

END
