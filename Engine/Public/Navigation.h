#pragma once
#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
private:
	explicit CNavigation(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>);
	explicit CNavigation(const CNavigation&);
	virtual ~CNavigation() DEFAULT;

public:
	HRESULT								Initialize_Prototype(const wstring& wstrDataFilePath);
	HRESULT								Initialize(const _uint iInitialCell, const _float4x4 mWorld = g_mUnit);
#ifdef _DEBUG
	virtual HRESULT						Render() override;
#endif

private:
	HRESULT								Ready_Neighbors();

public:
	_bool								Movable(const _float3 vPosition);

private:
	_uint								m_iCurrentCellIndex	= -1;
	vector<shared_ptr<class CCell>>		m_vecCell;

	_float4x4							m_mWorld			= g_mUnit;

#ifdef _DEBUG
	shared_ptr<class CShader>			m_pShader;
#endif

public:
	static shared_ptr<CNavigation>		Create(ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>, const wstring& wstrDataFilePath);
	virtual shared_ptr<CComponent>		Clone(any aNaviDesc) override;
};

END
