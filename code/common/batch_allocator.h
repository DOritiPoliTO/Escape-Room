#pragma once

#include <new>
#include <list>
#include <unordered_map>
#include <typeindex>

class BatchAllocator
{
public:
	static BatchAllocator& getInstance();

	template<class T>
	inline void setNumberOfBlocksPerBatch(int n);

	template<class T>
	inline void* allocateBlock();

	template<class T>
	inline void releaseBlock(void* pBlock);

private:
	struct Batch;
	struct BatchInfo;

	std::unordered_map<std::type_index, BatchInfo> BatchInfos_;
};

struct BatchAllocator::Batch
{
	char* pMemory;
	int bitmap;

	Batch(size_t size, int bitmapInitialValue);
	~Batch();
};

struct BatchAllocator::BatchInfo
{
	const int bitmapInitialValue;
	const int numberOfBlocksPerBatch;
	std::list<Batch> batches;

	BatchInfo();
	BatchInfo(int n);
};

template<class T>
inline void BatchAllocator::setNumberOfBlocksPerBatch(int n)
{
	if (n > 32)
	{
		throw std::exception("Trying to set the number of blocks per batch too high!");
	}

	std::type_index typeIndex = std::type_index(typeid(T));

	auto it = BatchInfos_.find(typeIndex);
	if (it != BatchInfos_.end())
	{
		throw std::exception("Trying to set the number of blocks per batch twice for the same class!");
	}

	BatchInfos_.insert(std::pair<std::type_index, BatchInfo>(typeIndex, BatchInfo(n)));
}

template<class T>
inline void* BatchAllocator::allocateBlock()
{
	size_t size = sizeof(T);

	BatchInfo& batchInfo = BatchInfos_[std::type_index(typeid(T))];

	const int bitmapInitialValue = batchInfo.bitmapInitialValue;
	const int numberOfBlocksPerBatch = batchInfo.numberOfBlocksPerBatch;
	std::list<Batch>& batches = batchInfo.batches;

	for (auto it = batches.begin(); it != batches.end(); it++)
	{
		if (it->bitmap != 0b11111111111111111111111111111111)
		{
			int mask = 0b00000000000000000000000000000001;
			for (size_t i = 0; i < numberOfBlocksPerBatch; i++)
			{
				int b = (*it).bitmap & mask;
				if (b == 0b00000000000000000000000000000000)
				{
					it->bitmap |= mask;

					return it->pMemory + i * size;
				}

				mask = mask << 1;
			}
		}
	}

	Batch batch(size * numberOfBlocksPerBatch, bitmapInitialValue);
	batch.bitmap |= 0b00000000000000000000000000000001;

	batches.push_front(batch);

	void* pBlockAddress = batch.pMemory;
	batch.pMemory = nullptr;

	return pBlockAddress;
}

template<class T>
inline void BatchAllocator::releaseBlock(void* pBlock)
{
	size_t size = sizeof(T);

	BatchInfo& batchInfo = BatchInfos_[std::type_index(typeid(T))];

	const int bitmapInitialValue = batchInfo.bitmapInitialValue;
	const int numberOfBlocksPerBatch = batchInfo.numberOfBlocksPerBatch;
	std::list<Batch>& batches = batchInfo.batches;

	for (auto it = batches.begin(); it != batches.end(); it++)
	{
		if (pBlock >= it->pMemory
			&& pBlock <= it->pMemory + numberOfBlocksPerBatch * size)
		{
			int s = (reinterpret_cast<char*>(pBlock) - it->pMemory) / size;

			int mask = (0b00000000000000000000000000000001 << s) ^ 0b11111111111111111111111111111111;

			it->bitmap &= mask;

			return;
		}
	}
}
