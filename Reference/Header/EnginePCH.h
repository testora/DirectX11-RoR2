// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef ENGINE_PCH_H
#define ENGINE_PCH_H

// add headers that you want to pre-compile here

#include <time.h>
#include <process.h>

#include <cmath>
#include <memory>
#include <iostream>

#include <typeinfo>
#include <typeindex>

using std::type_info;
using std::type_index;

using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

using std::make_unique;
using std::make_shared;

using std::cout;
using std::wcout;
using std::endl;

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <algorithm>
#include <functional>

using std::string;
using std::wstring;
using std::vector;
using std::list;
using std::map;
using std::set;
using std::unordered_map;
using std::unordered_set;
using std::bitset;

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#ifdef ENGINE_EXPORTS

#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"

#endif // ENGINE_EXPORTS

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

#endif // ENGINE_PCH_H
