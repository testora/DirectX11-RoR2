#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class CCollision_Manager final : public CSingleton<CCollision_Manager>
{
private:
	union COLLIDER_ID
	{
		struct
		{
			_uint	COL_A;
			_uint	COL_B;
		};
		_ullong		KEY;
	};

	typedef map<weak_ptr<class CGameObject>, weak_ptr<class CCollider>, std::owner_less<>>	COLLIDER_MAP;
	typedef vector<_bool>																	COLLISION_CHECK;
	typedef unordered_map<_ullong, _bool>													COLLISION_INFO;

private:
	explicit CCollision_Manager() DEFAULT;
	virtual ~CCollision_Manager() DEFAULT;

public:
	HRESULT						Reserve_Manager(const COLLISION_GROUP);

	void						Tick(_float fTimeDelta);

public:
	void						Reset_GroupCheck();
	void						Check_Group(COLLISION_GROUP, COLLISION_GROUP, _bool bCheck = true);

	void						Register_Collider(COLLISION_GROUP, shared_ptr<class CGameObject>, shared_ptr<class CCollider>);
	void						Delete_Collider(shared_ptr<class CGameObject>, _float fTimeDelta = 0.f);

private:
	void						Tick_CollisionGroup(_float fTimeDelta, COLLISION_GROUP, COLLISION_GROUP);


private:
	vector<COLLIDER_MAP>		m_vecColliderMap;
	vector<COLLISION_CHECK>		m_vecColisionCheck;

	COLLISION_INFO				m_umapCollisionInfo;

	COLLISION_GROUP				m_eMaxGroup				= static_cast<COLLISION_GROUP>(0);

	friend CSingleton<CCollision_Manager>;
};

END
