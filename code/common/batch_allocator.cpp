#include "batch_allocator.h"

BatchAllocator& BatchAllocator::getInstance()
{
	static BatchAllocator instance;
	return instance;
}

BatchAllocator::Batch::Batch(size_t size, int bitmapInitialValue) :
	pMemory(nullptr),
	bitmap(bitmapInitialValue)
{
	pMemory = new char[size];
}

BatchAllocator::Batch::~Batch()
{
	if (pMemory != nullptr)
	{
		delete[] pMemory;
		pMemory = nullptr;
	}
}

BatchAllocator::BatchInfo::BatchInfo() :
	numberOfBlocksPerBatch(32),
	bitmapInitialValue(0b11111111111111111111111111111111 << 32)
{
}

BatchAllocator::BatchInfo::BatchInfo(int n) :
	numberOfBlocksPerBatch(n),
	bitmapInitialValue(0b11111111111111111111111111111111 << n)
{
}
