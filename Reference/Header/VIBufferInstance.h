#pragma once
#include "Engine_Define.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferInstance abstract : public CVIBuffer
{
protected:
	explicit CVIBufferInstance(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const COMPONENT);
	explicit CVIBufferInstance(const CVIBufferInstance&);
	virtual ~CVIBufferInstance() DEFAULT;

public:
	HRESULT									Initialize();

	void									Update(function<void(void*, _uint iNumInstance)>);

protected:
	ComPtr<ID3D11Buffer>					m_pInstanceVB;

	_uint									m_iInstanceStride			= 0;

public:
	virtual shared_ptr<CComponent>			Clone(any = g_aNull) override	PURE;
};

END
