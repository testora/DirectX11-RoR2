#include "EnginePCH.h"
#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::VIBUFFER)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& _rhs)
	: CComponent(_rhs)
	, m_pVB				(_rhs.m_pVB)
	, m_pIB				(_rhs.m_pIB)
	, m_tBufferDesc		(_rhs.m_tBufferDesc)
	, m_tInitializeData	(_rhs.m_tInitializeData)
	, m_eTopology		(_rhs.m_eTopology)
	, m_eIndexFormat	(_rhs.m_eIndexFormat)
	, m_iNumVB			(_rhs.m_iNumVB)
	, m_iNumVertex	(_rhs.m_iNumVertex)
	, m_iVertexStride	(_rhs.m_iVertexStride)
	, m_iNumIndex		(_rhs.m_iNumIndex)
	, m_iIndexStride	(_rhs.m_iIndexStride)
{
}

HRESULT CVIBuffer::Initialize()
{
	m_vecVB.emplace_back(m_pVB);
	m_vecVertexStride.emplace_back(m_iVertexStride);
	m_vecVertexOffset.emplace_back(0);

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	m_pContext->IASetVertexBuffers(0, m_iNumVB, &m_vecVB.front(), m_vecVertexStride.data(), m_vecVertexOffset.data());
	m_pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pContext->DrawIndexed(m_iNumIndex, 0, 0);

	return S_OK;
}
