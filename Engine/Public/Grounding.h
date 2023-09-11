#pragma once
#include "Engine_Define.h"
#include "Behavior.h"

BEGIN(Engine)

class ENGINE_DLL CGrounding final : public CBehavior
{
private:
	explicit CGrounding();
	explicit CGrounding(const CGrounding&);
	virtual ~CGrounding() DEFAULT;

public:
	HRESULT							Initialize(shared_ptr<class CGameObject> pOwner, const wstring& wstrTerrainGridLayerTag);
	virtual void					Tick(_float fTimeDelta) override;
	virtual void					Late_Tick(_float fTimeDelta) override;

public:
	_bool							Is_Grounding() const	{ return m_bIsGrounding; }

private:
	_float3							Intersect_Terrain();

private:
	wstring							m_wstrTerrainGridLayerTag;
	_bool							m_bIsGrounding		= false;

	shared_ptr<class CTransform>	m_pOwnerTransform;
	shared_ptr<class CPhysics>		m_pOwnerPhysics;

public:
	static shared_ptr<CGrounding>	Create(shared_ptr<class CGameObject> pOwner, const wstring& wstrTerrainGridLayerTag);
	virtual shared_ptr<CBehavior>	Clone(any = g_aNull) override;
};

END
