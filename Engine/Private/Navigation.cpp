#include "EnginePCH.h"
#include "Navigation.h"
#include "Cell.h"
#ifdef _DEBUG
#include "PipeLine.h"
#include "Shader.h"
#endif

CNavigation::CNavigation(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext)
	: CComponent(_pDevice, _pContext, COMPONENT::NAVIGATION)
{
}

CNavigation::CNavigation(const CNavigation& _rhs)
	: CComponent(_rhs)
	, m_vecCell	(_rhs.m_vecCell)
	, m_mWorld	(_rhs.m_mWorld)
#ifdef _DEBUG
	, m_pShader	(_rhs.m_pShader)
#endif
{
}

HRESULT CNavigation::Initialize_Prototype(const wstring& _wstrDataFilePath)
{
#pragma region Read File

	std::ifstream inFile(_wstrDataFilePath, std::ios::binary);

	if (!inFile.is_open())
	{
		MSG_RETURN(E_FAIL, "CNavigation::Initialize", "Failed to Open File");
	}

	while (true)
	{
		POLYGON arrPoint{};
		
		inFile.read(reinterpret_cast<_byte*>(arrPoint.data()),	sizeof(POLYGON));
		if (0 == inFile.gcount())
		{
			break;
		}

		shared_ptr<CCell> pCell = CCell::Create(m_pDevice, m_pContext, static_cast<_uint>(m_vecCell.size()), arrPoint);
		if (nullptr == pCell)
		{
			MSG_RETURN(E_FAIL, "CNavigation::Initialize", "Failed to Create Cell");
		}

		m_vecCell.emplace_back(pCell);
	}

	if (inFile.fail() || inFile.eof())
	{
		inFile.clear();
		inFile.close();
		MSG_RETURN(E_FAIL, "CNavigation::Initialize", "Failed to Read File");
	}

	inFile.close();

#pragma endregion

	if (FAILED(Ready_Neighbors()))
	{
		MSG_RETURN(E_FAIL, "CNavigation::Initialize", "Failed to Ready_Neighbors");
	}

#ifdef _DEBUG

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("Bin/Resources/Shader/Pos.hlsl"), VTXPOS::tElements, VTXPOS::iNumElement);
	if (nullptr == m_pShader)
	{
		MSG_RETURN(E_FAIL, "CNavigation::Initialize", "Failed to Compile Shader");
	}

#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(const _uint _iInitialCell, const _float4x4 _mWorld)
{
	if (_float4x4{} != m_mWorld)
	{
		m_mWorld = _mWorld;
	}
	else
	{
		m_iCurrentCellIndex = _iInitialCell;
	}

	return S_OK;
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATWORLD, m_mWorld)))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATVIEW, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::VIEW))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}
	if (FAILED(m_pShader->Bind_Matrix(SHADER_MATPROJ, CPipeLine::Get_Instance()->Get_Transform(PIPELINE::PROJECTION))))
	{
		MSG_RETURN(E_FAIL, "CTransform::Bind_OnShader", "Failed to CShader::Bind_Matrix");
	}

	if (FAILED(m_pShader->BeginPass(0)))
	{
		MSG_RETURN(E_FAIL, "CNavigation::Render", "Failed to BeginPass");
	}

	if (-1 == m_iCurrentCellIndex)
	{
//		if (FAILED(m_pShader->Bind_Vector("g_vColor", _color(1.f, 1.f, 1.f, 1.f))))
//		{
//			MSG_RETURN(E_FAIL, "CNavigation::Render", "Failed to Bind_Vector: g_vColor");
//		}
//
		HRESULT hr = S_OK;
//
//		for (auto& pCell : m_vecCell)
//		{
//			if (FAILED(pCell->Render()))
//			{
//				MSG_CONTINUE("CNavigation::Render", "Failed to Render: Navigation Mesh");
//				hr = E_FAIL;
//			}
//		}
//
		return hr;
	}
	else
	{
		if (FAILED(m_pShader->Bind_Vector("g_vColor", _color(0.f, 1.f, 0.f, 1.f))))
		{
			MSG_RETURN(E_FAIL, "CNavigation::Render", "Failed to Bind_Vector: g_vColor");
		}

		if (FAILED(m_vecCell[m_iCurrentCellIndex]->Render()))
		{
			MSG_RETURN(E_FAIL, "CNavigation::Render", "Failed to Render: Object Cell");
		}
	}
	
	return S_OK;
}
#endif

HRESULT CNavigation::Ready_Neighbors()
{
	for (auto& pCellA : m_vecCell)
	{
		for (auto& pCellB : m_vecCell)
		{
			if (pCellA == pCellB)
			{
				continue;
			}

			if (pCellA->Contain_Points(pCellB, CCell::POINT::A, CCell::POINT::B))
			{
				pCellA->Set_Neighbor(CCell::LINE::AB, pCellB);
			}
			else if (pCellA->Contain_Points(pCellB, CCell::POINT::B, CCell::POINT::C))
			{
				pCellA->Set_Neighbor(CCell::LINE::BC, pCellB);
			}
			else if (pCellA->Contain_Points(pCellB, CCell::POINT::C, CCell::POINT::A))
			{
				pCellA->Set_Neighbor(CCell::LINE::CA, pCellB);
			}
		}
	}

	return S_OK;
}

_bool CNavigation::Movable(const _float3 _vPosition)
{
	if (m_iCurrentCellIndex == -1)
	{
		MSG_RETURN(false, "CNavigation::Movable", "Unmovable Object");
	}

	_uint iCurrentIndex		= m_iCurrentCellIndex;
	_uint iNeighborIndex	= m_vecCell[m_iCurrentCellIndex]->Relative_Index(_vPosition);

	if (iNeighborIndex == iCurrentIndex)
	{
		return true;
	}
	else
	{
		if (-1 == iNeighborIndex)
		{
			return false;
		}
		else
		{
			while (true)
			{
				iCurrentIndex	= iNeighborIndex;
				iNeighborIndex	= m_vecCell[iCurrentIndex]->Relative_Index(_vPosition);

				if (iCurrentIndex == iNeighborIndex)
				{
					m_iCurrentCellIndex = iCurrentIndex;
					return true;
				}

				if (-1 == iNeighborIndex)
				{
					return false;
				}
			}
		}
	}
}

shared_ptr<CNavigation> CNavigation::Create(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pContext, const wstring& _wstrDataFilePath)
{
	shared_ptr<CNavigation> pInstance = make_private_shared(CNavigation, _pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_wstrDataFilePath)))
	{
		MSG_RETURN(nullptr, "CNavigation::Create", "Failed to Initialize_Prototype");
	}

	return pInstance;
}

shared_ptr<CComponent> CNavigation::Clone(any _aNaviDesc)
{
	if (_aNaviDesc.has_value())
	{
		shared_ptr<CNavigation> pInstance = make_private_shared_copy(CNavigation, *this);

		NAVIGATIONDESC tDesc = any_cast<NAVIGATIONDESC>(_aNaviDesc);

		if (FAILED(pInstance->Initialize(tDesc.iInitialIndex, tDesc.mWorld)))
		{
			MSG_RETURN(nullptr, "CNavigation::Clone", "Failed to Initialize");
		}

		return pInstance;
	}
	else
	{
		MSG_RETURN(nullptr, "CNavigation::Clone", "Invalid Parameter");
	}
	
}
