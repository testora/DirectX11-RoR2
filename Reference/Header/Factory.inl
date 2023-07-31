#include "Timer.h"

template<>
shared_ptr<CTimer> CFactory::Create()
{
	shared_ptr<CTimer> pInstance = shared_ptr<CTimer>(new CTimer(), [](CTimer* p) { delete p; });

	return pInstance;
}