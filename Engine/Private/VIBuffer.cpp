#include "EnginePCH.h"
#include "VIBuffer.h"
#include "Picker.h"

CVIBuffer::CVIBuffer(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const COMPONENT _eComponent)
	: CComponent(_pDevice, _pContext, _eComponent)
	, m_eTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
	, m_eIndexFormat(DXGI_FORMAT_UNKNOWN)
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

_bool CVIBuffer::Intersect(_In_opt_ const _float4x4 _mWorld) const
{
	_float3 vOut;
	return Intersect(vOut, _mWorld);
}

_bool CVIBuffer::Intersect(_Out_ _float3& _vOut, _In_opt_ const _float4x4 _mWorld) const
{
	_bool bCollide = false;

	union { _ushort* _16; _uint* _32; } pIndices = { nullptr };

	auto lambda = [this, &_vOut, _mWorld](auto* ptr)
	{
		_uint iNumPrimitive = m_eTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ? m_iNumIndices / 3 : 0;

		for (_uint iIndices = 0; iIndices < iNumPrimitive; ++iIndices)
		{
			for (_uint iIndex = 0; iIndex < 3; ++iIndex)
			{
				ptr[iIndex] = reinterpret_cast<decltype(ptr)>(m_pIndices.get())[iIndex + iIndices * 3];
			}
			if (CPicker::Get_Instance()->Intersect(_vOut, m_pVertices[ptr[0]], m_pVertices[ptr[1]], m_pVertices[ptr[2]], _mWorld))
			{
				return true;
			}
		}
		return false;
	};

	m_eIndexFormat == DXGI_FORMAT_R16_UINT ?
		bCollide = lambda(pIndices._16 = Function::CreateDynamicArray<_ushort>(3).get()) :
		bCollide = lambda(pIndices._32 = Function::CreateDynamicArray<_uint>(3).get());

	return bCollide;
}