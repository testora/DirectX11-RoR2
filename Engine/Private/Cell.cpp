#include "EnginePCH.h"
#include "Cell.h"
#ifdef _DEBUG
#include "VIBuffer_Cell.h"
#endif

CCell::CCell(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const _uint _iCellIndex)
	: m_iCellIndex	(_iCellIndex)
	, m_pDevice		(_pDevice)
	, m_pContext	(_pContext)
{
}

HRESULT CCell::Initialize(const POLYGON _arrPoint)
{
	m_arrPoint	= _arrPoint;
	m_arrNeighborIndex.fill(-1);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
	if (nullptr == m_pVIBuffer)
	{
		MSG_RETURN(E_FAIL, "CCell::Render", "Nullptr Exception: m_pVIBuffer");
	}

	if (FAILED(m_pVIBuffer->Render()))
	{
		MSG_RETURN(E_FAIL, "CCell::Render", "Failed to CVIBuffer_Cell::Render");
	}

	return S_OK;
}
#endif

_bool CCell::Contain_Points(shared_ptr<CCell> _pCell, POINT _ePointA, POINT _ePointB) const
{
	auto iterA = std::find(m_arrPoint.begin(), m_arrPoint.end(), _pCell->m_arrPoint[IDX(_ePointA)]);
	auto iterB = std::find(m_arrPoint.begin(), m_arrPoint.end(), _pCell->m_arrPoint[IDX(_ePointB)]);

	return iterA != m_arrPoint.end() && iterB != m_arrPoint.end();
}

_uint CCell::Relative_Index(_float3 _vPosition)
{
	for (size_t i = 0; i < IDX(LINE::MAX); ++i)
	{
		_float3 vPointDir		= _float3(_vPosition - m_arrPoint[i]).normalize();
		_float3 vLineDir		= m_arrPoint[(i + 1) % IDX(POINT::MAX)] - m_arrPoint[i];
		_float3 vVerticalDir	= _float3(-vLineDir.z, 0.f, vPointDir.x).normalize();

		if (0 < XMVectorGetX(XMVector3Dot(vPointDir, vVerticalDir)))
		{
			return m_arrNeighborIndex[i];
		}
	}

	return m_iCellIndex;
}

shared_ptr<CCell> CCell::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const _uint _iCellIndex, const POLYGON _arrPoint)
{
	shared_ptr<CCell> pInstance = make_private_shared(CCell, _pDevice, _pContext, _iCellIndex);

	if (FAILED(pInstance->Initialize(_arrPoint)))
	{
		MSG_RETURN(nullptr, "CCell::Create", "Failed to Initialize");
	}

	return pInstance;
}
