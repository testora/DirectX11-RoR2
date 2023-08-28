#include "EnginePCH.h"
#include "VIBuffer_Terrain.h"
#include "HandleRAII.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CVIBuffer(_pDevice, _pContext, COMPONENT::VIBUFFER_TERRAIN)
{
}

HRESULT CVIBuffer_Terrain::Initialize(any _strHeightMapPath)
{
	wstring strHeightMapPath = any_cast<wstring>(_strHeightMapPath);

	HandleRAII hFile(CreateFile(strHeightMapPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0));
	if (!hFile.is_valid())
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Terrain::Initialize", "Failed to CreateFile");
	}

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;
	_ulong				dwByte	= 0;

	if (!ReadFile(hFile.get(), &fh, sizeof fh, &dwByte, nullptr))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Terrain::Initialize", "Failed to ReadFile");
	}
	if (!ReadFile(hFile.get(), &ih, sizeof ih, &dwByte, nullptr))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Terrain::Initialize", "Failed to ReadFile");
	}

	m_vNumVertices.x	= static_cast<_float>(ih.biWidth);
	m_vNumVertices.y	= static_cast<_float>(ih.biHeight);
	m_iNumVertices		= static_cast<_uint>(m_vNumVertices.x * m_vNumVertices.y);

	auto pPixel = Function::CreateDynamicArray<_ulong>(m_iNumVertices);

	if (!ReadFile(hFile.get(), pPixel.get(), sizeof(_ulong) * m_iNumVertices, &dwByte, nullptr))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Terrain::Initialize", "Failed to ReadFile");
	}

	m_iNumVB			= 1;
	m_iVertexStride		= sizeof(VTXPOSNORTEX);
	m_iNumIndices		= static_cast<_uint>((m_vNumVertices.x - 1) * (m_vNumVertices.y - 1) * 2 * 3);
	m_iIndexStride		= sizeof(_uint);
	m_eIndexFormat		= DXGI_FORMAT_R32_UINT;
	m_eTopology			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pVertices			= Function::CreateDynamicArray<_float3>(m_iNumVertices);
	m_pIndices			= Function::CreateDynamicArray<_uint>(m_iNumIndices);

#pragma region VERTEX_BUFFER

	auto pVertices = Function::CreateDynamicArray<VTXPOSNORTEX>(m_iNumVertices);

	for (size_t y = 0; y < m_vNumVertices.y; y++)
	{
		for (size_t x = 0; x < m_vNumVertices.x; x++)
		{
			_uint	iIndex	= static_cast<_uint>(m_vNumVertices.x * y + x);

			pVertices[iIndex].vPosition	= m_pVertices[iIndex] = _float3(static_cast<_float>(x), (pPixel[iIndex] & 0x000000ff) / 20.f, static_cast<_float>(y));
			pVertices[iIndex].vNormal	= _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexCoord	= _float2(static_cast<_float>(x) / (m_vNumVertices.x - 1.f), static_cast<_float>(y) / (m_vNumVertices.y - 1.f));
		}
	}

#pragma endregion

#pragma region INDEX_BUFFER

	auto pIndices = Function::CreateDynamicArray<_uint>(m_iNumIndices);

	_uint iNumIndices = 0;

	for (size_t y = 0; y < m_vNumVertices.y - 1; y++)
	{
		for (size_t x = 0; x < m_vNumVertices.x - 1; x++)
		{
			_vector	vSrc, vDst, vNor;

			_uint	iIndex = static_cast<_uint>(m_vNumVertices.x * y + x);
			_uint	iIndices[4] = {
				static_cast<_uint>(iIndex + m_vNumVertices.x),
				static_cast<_uint>(iIndex + m_vNumVertices.x + 1),
				static_cast<_uint>(iIndex + 1),
				static_cast<_uint>(iIndex)
			};

			pIndices[iNumIndices++]	= iIndices[0];
			pIndices[iNumIndices++]	= iIndices[1];
			pIndices[iNumIndices++]	= iIndices[2];

			vSrc	= _float3(pVertices[iIndices[1]].vPosition) - _float3(pVertices[iIndices[0]].vPosition);
			vDst	= _float3(pVertices[iIndices[2]].vPosition) - _float3(pVertices[iIndices[1]].vPosition);
			vNor	= XMVector3Cross(vSrc, vDst);

			pVertices[iIndices[0]].vNormal	= _float3(_float3(pVertices[iIndices[0]].vNormal) + vNor).normalize();
			pVertices[iIndices[1]].vNormal	= _float3(_float3(pVertices[iIndices[1]].vNormal) + vNor).normalize();
			pVertices[iIndices[2]].vNormal	= _float3(_float3(pVertices[iIndices[2]].vNormal) + vNor).normalize();

			pIndices[iNumIndices++]	= iIndices[0];
			pIndices[iNumIndices++]	= iIndices[2];
			pIndices[iNumIndices++]	= iIndices[3];

			vSrc	= _float3(pVertices[iIndices[2]].vPosition) - _float3(pVertices[iIndices[0]].vPosition);
			vDst	= _float3(pVertices[iIndices[3]].vPosition) - _float3(pVertices[iIndices[2]].vPosition);
			vNor	= XMVector3Cross(vSrc, vDst);

			pVertices[iIndices[0]].vNormal = _float3(_float3(pVertices[iIndices[0]].vNormal) + vNor).normalize();
			pVertices[iIndices[1]].vNormal = _float3(_float3(pVertices[iIndices[1]].vNormal) + vNor).normalize();
			pVertices[iIndices[2]].vNormal = _float3(_float3(pVertices[iIndices[2]].vNormal) + vNor).normalize();
		}
	}

#pragma endregion

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumVertices * m_iVertexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= m_iVertexStride;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pVertices.get();

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pVB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

	ZeroMemory(&m_tBufferDesc, sizeof m_tBufferDesc);
	m_tBufferDesc.ByteWidth				= m_iNumIndices * m_iIndexStride;
	m_tBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	m_tBufferDesc.BindFlags				= D3D11_BIND_INDEX_BUFFER;
	m_tBufferDesc.CPUAccessFlags		= 0;
	m_tBufferDesc.MiscFlags				= 0;
	m_tBufferDesc.StructureByteStride	= 0;

	ZeroMemory(&m_tInitializeData, sizeof m_tInitializeData);
	m_tInitializeData.pSysMem			= pIndices.get();

	memcpy(m_pIndices.get(), pIndices.get(), m_iNumIndices * m_iIndexStride);

	if (FAILED(m_pDevice->CreateBuffer(&m_tBufferDesc, &m_tInitializeData, m_pIB.GetAddressOf())))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to CreateBuffer");
	}

	if (FAILED(__super::Initialize()))
	{
		MSG_RETURN(E_FAIL, "CVIBuffer_Rect::Initialize", "Failed to Initialize");
	}

	return S_OK;
}

shared_ptr<CVIBuffer_Terrain> CVIBuffer_Terrain::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strHeightMapPath)
{
	shared_ptr<CVIBuffer_Terrain> pInstance = make_private_shared(CVIBuffer_Terrain, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_strHeightMapPath)))
	{
		MSG_RETURN(nullptr, "CVIBuffer_Terrain::Create", "Failed to Initialize");
	}

	return pInstance;
}

shared_ptr<CComponent> CVIBuffer_Terrain::Clone(any)
{
	return shared_from_this();
}
