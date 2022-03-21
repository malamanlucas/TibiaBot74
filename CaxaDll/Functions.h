#pragma once

#ifndef FUNCTIONS
#define FUNCTIONS

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "Overlay.h"
#include <time.h>

struct vec4
{
	float x, y, z, w;
};
struct vec3
{
	float x, y, z;
};
struct vec2
{
	int x, y;
};

namespace ProcessFunctions
{
	template < class TypeValue > TypeValue ReadMemory(DWORD Address);
	template < class TypeValue > TypeValue WriteMemory(DWORD Address);
}

#endif



