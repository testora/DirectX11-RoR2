#pragma once

#ifdef	ENGINE_EXPORTS
#define	ENGINE_DLL				_declspec(dllexport)
#else
#define	ENGINE_DLL				_declspec(dllimport)
#endif

#define D3DCOLOR_RGBA(r, g, b, a)						((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#pragma region MessageBox

#define MSG_BOX(TITLE, MESSAGE)							MessageBox(0, TEXT(MESSAGE), TEXT(TITLE), MB_OK)
#define MSG_BREAK(TITLE, MESSAGE)						{ MSG_BOX(TITLE, MESSAGE); break; }
#define MSG_CONTINUE(TITLE, MESSAGE)					{ MSG_BOX(TITLE, MESSAGE); continue; }
#define MSG_RETURN(VALUE, TITLE, MESSAGE)				return [&]() { MSG_BOX(TITLE, MESSAGE); return VALUE; }()

#pragma endregion

#pragma region Pointer

#define make_private_shared(ClassType, ...)								shared_ptr<ClassType>(new ClassType(__VA_ARGS__), [](ClassType * p) { delete p; })
#define make_private_shared_copy(ClassType, rhs)						shared_ptr<ClassType>(new ClassType(rhs), [](ClassType* p) { delete p; })

#define make_private_shared_derived(ReturnType, ClassType, ...)			shared_ptr<ReturnType>(new ClassType(__VA_ARGS__), [](ReturnType* p) { delete static_cast<ClassType*>(p); })
#define make_private_shared_copy_derived(ReturnType, ClassType, rhs)	shared_ptr<ReturnType>(new ClassType(rhs), [](ReturnType* p) { delete static_cast<ClassType*>(p); })

#pragma endregion
