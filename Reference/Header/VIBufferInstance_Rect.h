#pragma once
#include "Engine_Define.h"
#include "VIBufferInstance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstance_Rect final : public CVIBufferInstance
{
private:
	explicit CVIBufferInstance_Rect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBufferInstance_Rect(const CVIBufferInstance_Rect&);
	virtual ~CVIBufferInstance_Rect() DEFAULT;

public:
	HRESULT										Initialize_Prototype(_uint iNumInstance);
	HRESULT										Initialize(any = g_aNull);

public:
	static shared_ptr<CVIBufferInstance_Rect>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, _uint iNumInstance);
	virtual shared_ptr<CComponent>				Clone(any = g_aNull) override;
};

END
