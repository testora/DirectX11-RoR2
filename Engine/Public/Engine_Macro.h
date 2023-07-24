#pragma once

#ifdef	ENGINE_EXPORTS
#define	ENGINE_DLL				_declspec(dllexport)
#else
#define	ENGINE_DLL				_declspec(dllimport)
#endif

#define VK_MAX					0xFF

#define BEGIN(NAME)				namespace NAME {
#define END						}

#define PURE					= 0
#define DEFAULT					= default

#define IDX(ENUM)				static_cast<unsigned int>(ENUM)

#pragma region MessageBox

#define MSG_BOX(TITLE, MESSAGE)							MessageBox(0, TEXT(MESSAGE), TEXT(TITLE), MB_OK)
#define MSG_RETURN(VALUE, TITLE, MESSAGE)				return [&]() { MSG_BOX(TITLE, MESSAGE); return VALUE; }()

#pragma endregion
