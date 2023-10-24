#include "EnginePCH.h"
#include "Collision_Manager.h"
#include "Collider.h"
#include "GameObject.h"

HRESULT CCollision_Manager::Reserve_Manager(const COLLISION_GROUP _eMaxGroup)
{
	m_eMaxGroup = _eMaxGroup;

	m_vecColliderMap.resize(static_cast<size_t>(m_eMaxGroup));
	m_vecColisionCheck.resize(static_cast<size_t>(m_eMaxGroup));
	for (auto& vec : m_vecColisionCheck)
	{
		vec.resize(static_cast<size_t>(m_eMaxGroup));
	}

	return S_OK;
}

void CCollision_Manager::Tick(_float _fTimeDelta)
{
	for (auto& vec : m_vecColliderMap)
	{
		for (auto& pair : vec)
		{
			pair.second->Tick(_fTimeDelta);
		}
	}

	for (size_t nRow = 0; nRow < static_cast<size_t>(m_eMaxGroup); ++nRow)
	{
		for (size_t nCol = nRow; nCol < static_cast<size_t>(m_eMaxGroup); ++nCol)
		{
			if (m_vecColisionCheck[nRow][nCol])
			{
				Tick_CollisionGroup(_fTimeDelta, static_cast<COLLISION_GROUP>(nRow), static_cast<COLLISION_GROUP>(nCol));
			}
		}
	}
}

void CCollision_Manager::Reset_GroupCheck()
{
	for (auto& vec : m_vecColisionCheck)
	{
		std::fill(vec.begin(), vec.end(), false);
	}
}

void CCollision_Manager::Check_Group(COLLISION_GROUP _eGroupA, COLLISION_GROUP _eGroupB, _bool _bCheck)
{
	size_t nRow = static_cast<size_t>(_eGroupA);
	size_t nCol = static_cast<size_t>(_eGroupB);

	if (nCol < nRow)
	{
		std::swap(nRow, nCol);
	}

	m_vecColisionCheck[nRow][nCol] = _bCheck;
}

void CCollision_Manager::Tick_CollisionGroup(_float _fTimeDelta, COLLISION_GROUP _eGroupA, COLLISION_GROUP _eGroupB)
{
	for (auto& iterA : m_vecColliderMap[IDX(_eGroupA)])
	{
		for (auto& iterB : m_vecColliderMap[IDX(_eGroupB)])
		{
			shared_ptr<CGameObject>	pObjectA	= iterA.first;
			shared_ptr<CGameObject>	pObjectB	= iterB.first;
			shared_ptr<CCollider>	pColliderA	= iterA.second;
			shared_ptr<CCollider>	pColliderB	= iterB.second;

			// Self Collision
			if (pObjectA == pObjectB)
			{
				continue;
			}

			// Generate ID
			COLLIDER_ID tID{};
			tID.COL_A = pColliderA->Get_ID();
			tID.COL_B = pColliderB->Get_ID();
			auto iter = m_umapCollisionInfo.find(tID.KEY);

			// ID not Registered
			if (m_umapCollisionInfo.end() == iter)
			{
				m_umapCollisionInfo.emplace(tID.KEY, false);
				iter = m_umapCollisionInfo.find(tID.KEY);
			}

			// Condition: On
			if (pColliderA->Intersect(pColliderB))
			{
				// Condition: On -> On
				if (iter->second)
				{
					pColliderA->OnCollision(pObjectB, _fTimeDelta);
					pColliderB->OnCollision(pObjectA, _fTimeDelta);
				}
				// Condition: Off -> On
				else
				{
					pColliderA->OnCollisionEnter(pObjectB, _fTimeDelta);
					pColliderB->OnCollisionEnter(pObjectA, _fTimeDelta);
					iter->second = true;
				}
			}
			// Condition: Off
			else
			{
				// Condition: On -> Off
				if (iter->second)
				{
					pColliderA->OnCollisionExit(pObjectB, _fTimeDelta);
					pColliderB->OnCollisionExit(pObjectA, _fTimeDelta);
					iter->second = false;
				}
			}
		}
	}
}

void CCollision_Manager::Register_Collider(COLLISION_GROUP _eGroup, shared_ptr<CGameObject> _pObject, shared_ptr<CCollider> _pCollider)
{
	m_vecColliderMap[static_cast<size_t>(_eGroup)][_pObject] = _pCollider;
}

void CCollision_Manager::Delete_Collider(shared_ptr<CGameObject> _pObject, _float _fTimeDelta)
{
	for (size_t nRow = 0; nRow < static_cast<size_t>(m_eMaxGroup); ++nRow)
	{
		for (size_t nCol = nRow; nCol < static_cast<size_t>(m_eMaxGroup); ++nCol)
		{
			for (auto& iterA : m_vecColliderMap[nRow])
			{
				for (auto& iterB : m_vecColliderMap[nCol])
				{
					shared_ptr<CGameObject>	pObjectA	= iterA.first;
					shared_ptr<CGameObject>	pObjectB	= iterB.first;
					shared_ptr<CCollider>	pColliderA	= iterA.second;
					shared_ptr<CCollider>	pColliderB	= iterB.second;

					COLLIDER_ID tID{};
					tID.COL_A = pColliderA->Get_ID();
					tID.COL_B = pColliderB->Get_ID();
					auto iter = m_umapCollisionInfo.find(tID.KEY);

					if (m_umapCollisionInfo.end() != iter)
					{
						if (pColliderA->Intersect(pColliderB))
						{
							if (iter->second)
							{
								pColliderA->OnCollisionExit(pObjectB, _fTimeDelta);
								pColliderB->OnCollisionExit(pObjectA, _fTimeDelta);
								iter->second = false;
							}
						}
					}
				}
			}
		}
	}

	for (auto& vec : m_vecColliderMap)
	{
		if (vec.end() != vec.find(_pObject))
		{
			vec.erase(_pObject);
		}
	}
}
