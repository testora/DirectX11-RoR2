// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <time.h>
#include <process.h>

#include <cmath>
#include <memory>
#include <iostream>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;

#ifdef ENGINE_EXPORTS

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"

#endif //ENGINE_EXPORTS

#include "EnTT/entt.hpp"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif //_DEBUG

#endif //PCH_H
