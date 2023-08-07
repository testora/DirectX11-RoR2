#include "EnginePCH.h"
#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const COMPONENT _eComponent)
	: CComponent(_pDevice, _pContext, _eComponent)
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
	, m_iNumVertices	(_rhs.m_iNumVertices)
	, m_iVertexStride	(_rhs.m_iVertexStride)
	, m_iNumIndices		(_rhs.m_iNumIndices)
	, m_iIndexStride	(_rhs.m_iIndexStride)
{
}

HRESULT CVIBuffer::Initialize(any)
{
	m_vecVB.emplace_back(m_pVB);
	m_vecVertexStride.emplace_back(m_iVertexStride);
	m_vecVertexOffset.emplace_back(0);

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	m_pContext->IASetVertexBuffers(0, m_iNumVB, Function::ConvertToRawPtrVector(m_vecVB).data(), m_vecVertexStride.data(), m_vecVertexOffset.data());
	m_pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}
