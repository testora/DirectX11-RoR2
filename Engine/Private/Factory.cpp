#include "EnginePCH.h"
#include "Factory.h"
#pragma region Header
#include "GameObject.h"
#include "Timer.h"
#include "ObjectLayer.h"
#include "ObjectPool.h"
#pragma endregion

template<>
shared_ptr<CTimer> CFactory::Create<CTimer>()
{
	return shared_ptr<CTimer>(new CTimer(), [](CTimer* p) { delete p; });
}

template<>
shared_ptr<CObjectLayer> CFactory::Create<CObjectLayer>()
{
	return shared_ptr<CObjectLayer>(new CObjectLayer(), [](CObjectLayer* p) { delete p; });
}

template<>
shared_ptr<CObjectPool> CFactory::Create<CObjectPool>(const _uint _iPoolSize, shared_ptr<class CGameObject> _pObject)
{
	shared_ptr<CObjectPool> pInstance = shared_ptr<CObjectPool>(new CObjectPool(_iPoolSize), [](CObjectPool* p) { delete p; });

	if (FAILED(pInstance->Initialize(_pObject)))
	{
		MSG_RETURN(nullptr, "CFactory::Create", "Failed to Initialize");
	}

	return pInstance;
}

