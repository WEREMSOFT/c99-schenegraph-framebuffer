#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <assert.h>
#include <stdlib.h>
#include "logger.h"
#include "debugMemory.h"

#ifndef myMalloc
	#define myMalloc __my_malloc__
	#define myFree __my_free__
#endif

// Array
typedef struct
{
	size_t size;
	size_t capacity;
	size_t dataTypeSize;
	char data[1];
} ArrayHeader;

ArrayHeader* arrayCreate(size_t capacity, size_t dataTypeSize)
{
	assert(capacity > 0 && "Capacity can not be 0 (or less)");
	ArrayHeader* header = myMalloc(sizeof(ArrayHeader) + capacity * dataTypeSize);
	header->capacity = capacity;
	header->size = 0;
	header->dataTypeSize = dataTypeSize;
	return header;
}

ArrayHeader* arrayCreateAndInitToZero(size_t capacity, size_t dataTypeSize)
{
	assert(capacity > 0 && "Capacity can not be 0 (or less)");
	size_t size = sizeof(ArrayHeader) + capacity * dataTypeSize;
	ArrayHeader* returnValue = myMalloc(size);

	memset(returnValue->data, 0, capacity * dataTypeSize);
	
	ArrayHeader* header = (ArrayHeader*)returnValue;
	header->capacity = capacity;
	header->size = 0;
	header->dataTypeSize = dataTypeSize;
	return returnValue;
}

void arrayDestroy(void *array)
{
	myFree(array);
}

void arrayClear(ArrayHeader* _this)
{
	_this->size = 0;

	#ifdef __DEBUG_BUILD__
		memset(&_this->data[0], 0, _this->capacity * _this->dataTypeSize);
	#endif
}

ArrayHeader* arrayAddElement(ArrayHeader* _this, const void* element)
{
	#ifdef __DEBUG_BUILD__
	assert(element > 1024 && "the pointer doesn't look like a pointer");
	#endif
	if(_this->size == _this->capacity)
	{
		size_t newCapacity = _this->capacity * 2;
		size_t fullNewSize = sizeof(ArrayHeader) + newCapacity * _this->dataTypeSize;
		_this = myRealloc(_this, fullNewSize);
		_this->capacity = newCapacity;
	}

	typedef struct 
	{
		char array[_this->dataTypeSize];
	} Pivot;


	Pivot* pivot = (Pivot *)&_this->data[_this->size * _this->dataTypeSize];

   	*pivot = *((Pivot*)element);

	_this->size += 1;
	return _this;
}

ArrayHeader* arrayAddElementI(ArrayHeader* _this, int element)
{
	assert(_this->dataTypeSize == sizeof(int) && "array does not contains ints");
	_this = arrayAddElement(_this, &element);
	return _this;
}

ArrayHeader* arrayAddElementF(ArrayHeader* _this, float element)
{
	assert(_this->dataTypeSize == sizeof(float) && "array does not contains floats");
	_this = arrayAddElement(_this, &element);
	return _this;
}

ArrayHeader* arrayAddElementAt(ArrayHeader* _this, const void* element, int index)
{
	if(index >= _this->capacity)
	{
		size_t newCapacity = index * 2;
		size_t fullNewSize = sizeof(ArrayHeader) + newCapacity * _this->dataTypeSize;
		_this = myRealloc(_this, fullNewSize);
		assert(_this != NULL && "realloc error");
		_this->capacity = newCapacity;
		_this->size = index;
	}

	memcpy(&_this->data[index * _this->dataTypeSize], element, _this->dataTypeSize);
	
	// The size of the array must be at least equal to index;
	if(_this->size == index)
	{
		_this->size++;
	}

	if(_this->size < index)
	{
		_this->size = index + 1;
	}

	return _this;
}

void* arrayGetElementAt(ArrayHeader* _this, int index)
{
	assert(index > -1 && "arrayGetElementAt negative index");
	assert(index <= _this->size && "arrayGetElementAt index out of bounds");
	return &_this->data[index * _this->dataTypeSize];
}

int arrayGetElementAtI(ArrayHeader* _this, int index)
{
	assert(_this->dataTypeSize == sizeof(int) && "array does not contains ints");
	int *element = arrayGetElementAt(_this, index);
	return *element;
}

float arrayGetElementAtf(ArrayHeader* _this, int index)
{
	assert(_this->dataTypeSize == sizeof(float) && "array does not contains floats");
	float *element = arrayGetElementAt(_this, index);
	return *element;
}

void* arrayGetElementOrCreateAt(ArrayHeader** _this, int index)
{

	if(index >= (*_this)->size)
	{
		typedef struct 
		{
			char array[(*_this)->dataTypeSize];
		} Pivot;

		Pivot pivot;
		memset(&pivot, 0, (*_this)->dataTypeSize);
		(*_this) = arrayAddElementAt(*_this, &pivot, index);
	}
	return arrayGetElementAt((*_this), index);
}

void arrayDeleteElement(ArrayHeader *_this, int elementIndex)
{
	if(_this->size == 0 || elementIndex >= _this->size) return;
	
	typedef struct 
	{
		char array[_this->dataTypeSize];
	} Pivot;

	int lastIndex = _this->size - 1;
	Pivot* lastElement = (Pivot *)&_this->data[lastIndex * _this->dataTypeSize];
	*((Pivot *)&_this->data[elementIndex * _this->dataTypeSize]) = *lastElement;

	#ifdef __DEBUG_BUILD__
		memset(&_this->data[lastIndex * _this->dataTypeSize], 0, _this->dataTypeSize);
	#endif

	_this->size--;
}
#endif