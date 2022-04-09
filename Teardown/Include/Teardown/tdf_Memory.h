#pragma once

#include "Memory.h"
#include "Signatures.h"
#include "Logger.h"

typedef void	(*tfree)		(void* memBlock);
typedef void* (*tmalloc_base)	(size_t size);

namespace Teardown
{
	namespace MemoryFunctions
	{
		inline tfree Free;
		inline tmalloc_base Alloc;

		inline void GetAddresses()
		{
			Free = (tfree)Memory::dwFindPattern(Signatures::free_base);
			Alloc = (tmalloc_base)Memory::dwFindPattern(Signatures::malloc_base);
		}
	}
}