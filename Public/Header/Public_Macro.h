#pragma once

#define VK_MAX					0xFF

#define BEGIN(NAME)				namespace NAME {
#define END						}

#define DEFAULT					= default

#define IDX(ENUM)				static_cast<unsigned int>(ENUM)
#define BIT(ENUM)				static_cast<unsigned int>(1u << IDX(ENUM))
