#pragma once
#include <cassert>
#include <cstring>

// Prototypes
bool FileExists(char const* filename);

// header-only code
template<typename T> void ZeroMem(T* obj)
{
	assert(obj);
	std::memset(obj, 0, sizeof(T)); //mk: Sets the first num bytes (sizeof T) of the block of memory pointed by ptr (obj) to the specified value (interpreted as an unsigned char; 0)
}

