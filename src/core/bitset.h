#ifndef __BITSET_H__
#define __BITSET_H__

#include <stdint.h>

#define BITSET_TYPE uint32_t

typedef BITSET_TYPE Bitset;

void bitsetSet(Bitset* _this, int bitIndex)
{
	Bitset bitToSet = 1;
	bitToSet <<= bitIndex;

	(*_this) |= bitToSet;
}

int bitsetIsSet(Bitset _this, int bitIndex)
{
	Bitset bitToTest = 1;
	bitToTest <<= bitIndex;

	return (_this & bitToTest) == bitToTest;
}

void bitsetClear(Bitset* _this, int bitIndex)
{
	Bitset bitToSet = 1;
	
	bitToSet <<= bitIndex;

	if(*_this & bitToSet == 0) return _this;

	(*_this) ^= bitToSet;
}

#endif