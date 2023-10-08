#pragma once
#include "Engine_Define.h"
#include "VIBufferInstance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstance_Point final : public CVIBufferInstance
{
private:
	explicit CVIBufferInstance_Point(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBufferInstance_Point(const CVIBufferInstance_Point&);
	virtual ~CVIBufferInstance_Point() DEFAULT;

public:
	HRESULT										Initialize_Prototype(_uint iNumInstance);
	HRESULT										Initialize(any = g_aNull);

public:
	static shared_ptr<CVIBufferInstance_Point>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _uint iNumInstance);
	virtual shared_ptr<CComponent>				Clone(any = g_aNull) override;
};

END
