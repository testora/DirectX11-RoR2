#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCell final
{
public:
	enum class POINT	{ A, B, C, MAX };
	enum class LINE		{ AB, BC, CA, MAX };

private:
	explicit CCell(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _uint iCellIndex);
	virtual ~CCell() DEFAULT;

public:
	HRESULT								Initialize(const POLYGON);
#ifdef _DEBUG
	HRESULT								Render();
#endif

public:
	_bool								Contain_Points(shared_ptr<CCell>, POINT, POINT) const;
	_uint								Relative_Index(_float3 vPosition);
	void								Set_Neighbor(LINE eLine, shared_ptr<CCell> pNeighbor)	{ m_arrNeighborIndex[IDX(eLine)] = pNeighbor->m_iCellIndex; }

private:
	const _uint							m_iCellIndex	= 0;
	POLYGON								m_arrPoint{};
	array<_uint, IDX(LINE::MAX)>		m_arrNeighborIndex{};

#ifdef _DEBUG
	shared_ptr<class CVIBuffer_Cell>	m_pVIBuffer;
#endif

	ComPtr<ID3D11Device>				m_pDevice;
	ComPtr<ID3D11DeviceContext>			m_pContext;

public:
	static shared_ptr<CCell>			Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const _uint iCellIndex, const POLYGON);
};

END
