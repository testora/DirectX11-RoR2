#pragma once
#include "Engine_Define.h"
#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CParallel : public CComposite
{
public:
	enum class POLICY	{ REQUIRE_ONE, REQUIRE_ALL, MAX };

private:
	explicit CParallel(POLICY eSuccess, POLICY eFailure);
	virtual ~CParallel() DEFAULT;

public:
	virtual HRESULT						Initialize() override;
	virtual STATUS						Tick(_float fTimeDelta) override;
	virtual HRESULT						Terminate() override;

private:
	const POLICY						m_eSuccessPolicy	= POLICY::MAX;
	const POLICY						m_eFailurePolicy	= POLICY::MAX;
};

END
