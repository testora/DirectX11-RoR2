#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	explicit CVIBuffer(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CVIBuffer(const CVIBuffer&);
	virtual ~CVIBuffer() DEFAULT;

public:
	virtual HRESULT					Initialize()	PURE;
	HRESULT							Render();

protected:
	ComPtr<ID3D11Buffer>			m_pVB;
	ComPtr<ID3D11Buffer>			m_pIB;

	D3D11_BUFFER_DESC				m_tBufferDesc;
	D3D11_SUBRESOURCE_DATA			m_tInitializeData;
	D3D11_PRIMITIVE_TOPOLOGY		m_eTopology;
	DXGI_FORMAT						m_eIndexFormat;

	_uint							m_iNumVB;
	_uint 							m_iNumVertex;
	_uint							m_iVertexStride;
	_uint 							m_iNumIndex;
	_uint							m_iIndexStride;

	vector<ComPtr<ID3D11Buffer>>	m_vecVB;
	vector<_uint>					m_vecVertexStride;
	vector<_uint>					m_vecVertexOffset;

public:
	virtual shared_ptr<CComponent>	Clone(std::any = nullptr) override	PURE;
};

END
