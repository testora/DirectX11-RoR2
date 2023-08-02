#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer, public std::enable_shared_from_this<CVIBuffer_Rect>
{
private:
	explicit CVIBuffer_Rect(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect&);
	virtual ~CVIBuffer_Rect() DEFAULT;

public:
	virtual HRESULT						Initialize() override;

public:
	static shared_ptr<CVIBuffer_Rect>	Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	virtual shared_ptr<CComponent>		Clone(std::any = nullptr) override;
};

END
