#pragma once

#define VK_MAX					0xFF

#define BEGIN(NAME)				namespace NAME {
#define END						}

#define PURE					= 0
#define DEFAULT					= default

#define IDX(ENUM)				static_cast<unsigned int>(ENUM)
