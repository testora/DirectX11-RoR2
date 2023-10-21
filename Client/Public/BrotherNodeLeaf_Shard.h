#pragma once
#include "Client_Define.h"
#include "Node.h"

BEGIN(Engine)
class CObjectPool;
class CTransform;
class CModel;
class CAnimator;
END

BEGIN(Client)

class CBrotherNodeLeaf_Shard final : public CNode
{
private:
	explicit CBrotherNodeLeaf_Shard() DEFAULT;
	virtual ~CBrotherNodeLeaf_Shard() DEFAULT;

public:
	virtual HRESULT								Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void								Activate() override;
	virtual STATUS								Invoke(_float fTimeDelta) override;
	virtual void								Terminate() override;

private:
	shared_ptr<CTransform>						m_pTransform;
	shared_ptr<CModel>							m_pModel;
	shared_ptr<CAnimator>						m_pAnimator;

	shared_ptr<CObjectPool>						m_pPool;

	_float4x4									m_mPivot;
	const _float4x4*							m_pShardPoint	= nullptr;

	_float										m_fShardTime	= 0.f;

public:
	static shared_ptr<CBrotherNodeLeaf_Shard>	Create(shared_ptr<class CBlackBoard>);
};

END
