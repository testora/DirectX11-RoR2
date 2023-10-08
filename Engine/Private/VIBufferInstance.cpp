#include "EnginePCH.h"
#include "VIBufferInstance.h"

CVIBufferInstance::CVIBufferInstance(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const COMPONENT _eComponent)
	: CVIBuffer(_pDevice, _pContext, _eComponent)
{
}

CVIBufferInstance::CVIBufferInstance(const CVIBufferInstance& _rhs)
	: CVIBuffer					(_rhs)
	, m_pInstanceVB				(_rhs.m_pInstanceVB)
	, m_iInstanceStride			(_rhs.m_iInstanceStride)
{
}

HRESULT CVIBufferInstance::Initialize()
{
	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBufferInstance::Initialize", "Failed to CVIBuffer::Initialize");
	}

	m_vecVB.emplace_back(m_pInstanceVB);
	m_vecVertexStride.emplace_back(m_iInstanceStride);
	m_vecVertexOffset.emplace_back(0);

	return S_OK;
}

void CVIBufferInstance::Update(function<void(void* _pData, _uint _iNumInstance)> _fpListener)
{
	D3D11_MAPPED_SUBRESOURCE tSubResource;

	m_pContext->Map(m_pInstanceVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &tSubResource);

	for (_uint i = 0; i < m_iNumInstance; i++)
	{
		_fpListener(tSubResource.pData, m_iNumInstance);
	}

	m_pContext->Unmap(m_pInstanceVB.Get(), 0);
}
