#include "EnginePCH.h"
#include "VIBuffer.h"
#include "Shader.h"
#include "PipeLine.h"
#include "Picker.h"
#include "Bounding_AABB.h"
#include "Component_Manager.h"
#include "Scene_Manager.h"

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

#if WIP_FRUSTRUM_CULLING
	_vector vMin = XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	_vector vMax = XMVectorSet(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);
	Iterate_Polygons(
		[&](_float3 _v0, _float3 _v1, _float3 _v2)->_bool
		{
			vMin = XMVectorMin(vMin, XMVectorMin(_v0, XMVectorMin(_v1, _v2)));
			vMax = XMVectorMax(vMax, XMVectorMax(_v0, XMVectorMax(_v1, _v2)));
			return true;
		}
	);

	BoundingBox::CreateFromPoints(m_tBoundingBox, vMin, vMax);
#endif

	return S_OK;
}

HRESULT CVIBuffer::Render(shared_ptr<CShader> _pShader, _uint _iPassIndex)
{
	if (FAILED(_pShader->BeginPass(_iPassIndex)))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer::Render", "Failed to CShader::BeginPass");
	}

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

	switch (m_eIndexFormat)
	{
	case DXGI_FORMAT_R16_UINT:
	{
		auto arr = Function::CreateDynamicArray<_ushort>(3);
		bCollide = lambda(arr.get());
			break;
	}
		
	case DXGI_FORMAT_R32_UINT:
	{
		auto arr = Function::CreateDynamicArray<_uint>(3);
		bCollide = lambda(arr.get());
		break;
	}

	default:
		MSG_RETURN(false, "CVIBuffer::Intersect", "Invalid Index Format");
	}

	return bCollide;
}

void CVIBuffer::Iterate_Polygons(function<_bool(POLYGON)> _funcCallback)
{
	for (size_t i = 0; i < m_iNumIndices; i += 3)
	{
		_float3 v0 = m_pVertices[m_pIndices[i + 0]];
		_float3 v1 = m_pVertices[m_pIndices[i + 1]];
		_float3 v2 = m_pVertices[m_pIndices[i + 2]];

		if (!_funcCallback(POLYGON{ v0, v1, v2 }))
		{
			return;
		}
	}
}

void CVIBuffer::Iterate_Indices(function<_bool(_uint, _uint, _uint)> _funcCallback)
{
	for (size_t i = 0; i < m_iNumIndices; i += 3)
	{
		if (!_funcCallback(m_pIndices[i], m_pIndices[i + 1], m_pIndices[i + 2]))
		{
			return;
		}
	}
}
