#pragma once
#include "Engine_Define.h"
#include "Composite.h"

BEGIN(Engine)

class ENGINE_DLL CParallel abstract : public CComposite
{
public:
	enum class POLICY	{ REQUIRE_ONE, REQUIRE_ALL, MAX };

protected:
	explicit CParallel(POLICY eSuccess, POLICY eFailure);
	virtual ~CParallel() DEFAULT;

public:
	virtual void						Activate() override;
	virtual STATUS						Invoke(_float fTimeDelta) override;
	virtual void						Terminate() override;

private:
	const POLICY						m_eSuccessPolicy	= POLICY::MAX;
	const POLICY						m_eFailurePolicy	= POLICY::MAX;
};

END
