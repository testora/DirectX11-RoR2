#pragma once
#include "Engine_Define.h"
#include "VIBufferInstance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstance_Line final : public CVIBufferInstance
{
private:
	explicit CVIBufferInstance_Line(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBufferInstance_Line(const CVIBufferInstance_Line&);
	virtual ~CVIBufferInstance_Line() DEFAULT;

private:
	HRESULT										Initialize_Prototype(_uint iNumInstance);
	HRESULT										Initialize(any = g_aNull);

public:
	static shared_ptr<CVIBufferInstance_Line>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _uint iNumInstance);
	virtual shared_ptr<CComponent>				Clone(any = g_aNull) override;
};

END
