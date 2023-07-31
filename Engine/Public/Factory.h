#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CFactory final
{
public:
	template <typename T, typename... Args>
	static shared_ptr<T> Create(Args... args)
	{
		shared_ptr<T> pInstance = shared_ptr<T>(new T(), [](T* p) { delete p; });

		if (FAILED(pInstance->Initialize_Prototype(args...)))
		{
			MSG_RETURN(E_FAIL, "CFactory::Create", "Failed to Initialize_Prototype");
		}

		auto iter = m_umapPrototype.find(type_index(typeid(T)));
		if (iter == m_umapPrototype.end())
		{
			m_umapPrototype.emplace(type_index(typeid(T)), pInstance);
		}

		return pInstance;
	}

	template <typename T, typename Device, typename Context, typename... Args>
	static shared_ptr<T> CreateDev(Device device, Context context, Args... args)
	{
		shared_ptr<T> pInstance = shared_ptr<T>(new T(device, context), [](T* p) { delete p; });

		if (FAILED(pInstance->Initialize_Prototype(args...)))
		{
			MSG_RETURN(E_FAIL, "CFactory::CreateDev", "Failed to Initialize_Prototype");
		}

		auto iter = m_umapPrototype.find(type_index(typeid(T)));
		if (iter == m_umapPrototype.end())
		{
			m_umapPrototype.emplace(type_index(typeid(T)), pInstance);
		}

		return pInstance;
	}

	template <typename T, typename... Args>
	static shared_ptr<T> Clone(Args... args)
	{
		shared_ptr<T> pInstance = shared_ptr<T>(new T(m_umapPrototype[type_index(typeid(T))]));

		if (FAILED(pInstance->Initialize(args...)))
		{
			MSG_RETURN(E_FAIL, "CFactory::Clone", "Failed to Initialize");
		}

		return pInstance;
	}

	unordered_map<type_index, shared_ptr<class CGameObject>>	m_umapPrototype;
};

template<> shared_ptr<class CTimer>			CFactory::Create<CTimer>();
template<> shared_ptr<class CObjectLayer>	CFactory::Create<CObjectLayer>();
template<> shared_ptr<class CObjectPool>	CFactory::Create<CObjectPool>(const _uint iPoolSize, shared_ptr<class CGameObject>);

END
