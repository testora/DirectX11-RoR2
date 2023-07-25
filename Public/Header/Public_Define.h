#pragma once
#pragma warning(disable: 4005)
#pragma warning(disable: 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#include <cmath>
#include <memory>
#include <iostream>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

#include "Public_Macro.h"
#include "Public_Typedef.h"

#include "Singleton.h"
