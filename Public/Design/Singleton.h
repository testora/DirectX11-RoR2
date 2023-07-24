#pragma once
#include "Public_Define.h"

template<typename T>
class CSingleton abstract
{
public:
	CSingleton(const CSingleton&)					= delete;
	CSingleton(CSingleton&&) noexcept				= delete;
	CSingleton& operator=(const CSingleton&)		= delete;
	CSingleton& operator=(CSingleton&&) noexcept	= delete;

protected:
	explicit CSingleton()							= default;
	virtual ~CSingleton()							= default;

public:
	static shared_ptr<T> Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = shared_ptr<T>(new T(), [](T* p) { delete p; });
		return m_pInstance;
	}

	static _long Destroy_Instance()
	{
		_long count = m_pInstance.use_count();
		m_pInstance.reset();
		return --count;
	}

private:
	static shared_ptr<T> m_pInstance;
};

template<typename T>
shared_ptr<T> CSingleton<T>::m_pInstance = nullptr;
