#ifndef __DEBUG_MEMORY__
#define __DEBUG_MEMORY__

	#include <stdlib.h>
	#include <assert.h>
	#include "logger.h"

	#define MAX_INFO_COUNT 500

	#define SHOW_MEM_DEBUG_INFO

	typedef struct 
	{
		void* pointer;
		size_t size;
	} MemInfo;
	static int meminfoCount = 0;
	static MemInfo memoryInformations[MAX_INFO_COUNT];

	static int totalAllocatedMemory = 0;

	int getMemoryInfoIndexFromPointer(void* p)
	{
		for(int i = 0; i<meminfoCount; i++)
		{
			if(memoryInformations[i].pointer == p)
			{
				return i;
			}
		}
		assert("memory location not found, possible double free?");
	}

	MemInfo getMemoryInfoFromPointer(void* p)
	{
		int index = getMemoryInfoIndexFromPointer(p);
		return memoryInformations[index];
	}

	void* debug_realloc(void *p, size_t size, char *file, int line)
	{
		totalAllocatedMemory += size;
		#ifdef SHOW_MEM_DEBUG_INFO
			loggerLog("reallocating %d bytes at %s:%d", size, file, line);
			loggerLog("Total allocated memory %zu", totalAllocatedMemory);
			loggerLog("meminfo count %d", meminfoCount);
		#endif
		MemInfo memInfo = {0};
		memInfo.pointer = realloc(p, size);
		memInfo.size = size;

		int location = getMemoryInfoIndexFromPointer(p);

		memoryInformations[location] = memInfo;

		return memInfo.pointer;
	}

	void* debug_malloc(size_t size, char *file, int line)
	{
		totalAllocatedMemory += size;
		#ifdef SHOW_MEM_DEBUG_INFO
			loggerLog("allocating %d bytes at %s:%d", size, file, line);
			loggerLog("Total allocated memory %zu", totalAllocatedMemory);
		#endif
		MemInfo memInfo = {0};
		memInfo.pointer = malloc(size);
		memInfo.size = size;

		memoryInformations[meminfoCount++] = memInfo;

		assert(meminfoCount < MAX_INFO_COUNT && "MAX debug information elements reached");

		return memInfo.pointer;
	}

	void* debug_calloc(size_t nmemb, size_t size, char *file, int line)
	{
		totalAllocatedMemory += size;
		#ifdef SHOW_MEM_DEBUG_INFO
			loggerLog("allocating %d bytes at %s:%d", size, file, line);
			loggerLog("Total allocated memory %zu", totalAllocatedMemory);
		#endif
		MemInfo memInfo = {0};
		memInfo.pointer = calloc(nmemb, size);
		memInfo.size = nmemb * size;

		memoryInformations[meminfoCount++] = memInfo;

		assert(meminfoCount < MAX_INFO_COUNT && "MAX debug information elements reached");

		return memInfo.pointer;
	}

	void debug_free(void* p, char *file, int line)
	{
		int location = getMemoryInfoIndexFromPointer(p);

		assert(location > -1 && "pointer not found, possible double free?");
		#ifdef SHOW_MEM_DEBUG_INFO
			loggerLog("deallocating %zu bytes - %s:%d", memoryInformations[location].size, file, line);
		#endif

		memoryInformations[location] = memoryInformations[meminfoCount-1];
		meminfoCount--;
		free(p);
	}

	int getTotalAllocatedMemory()
	{
		int result = 0;
		for(int i = 0; i < meminfoCount; i++)
		{
			result += memoryInformations[i].size;
		}
		return result;
	}

	#define malloc(x) debug_malloc((x), __FILE__, __LINE__)
	#define realloc(x, y) debug_realloc((x), (y), __FILE__, __LINE__)
	#define free(x) debug_free((x), __FILE__, __LINE__)
	#define calloc(x, y) debug_calloc((x),  (y), __FILE__, __LINE__)
#endif