#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace Storm::Core;

BlockAllocator::BlockAllocator(size_t blockSize, size_t capacity)
	:mData(nullptr)
	,mBlockSize(blockSize)
	,mCapacity(capacity)
	,mFreeBlocks(capacity)
{
	// TODO
	ASSERT(blockSize > 0, "BlockAllocator -- Invalid block size!");
	ASSERT(capacity > 0, "BlockAllocator -- Invalid block capacity!");

	//Preallocate memory
	mData = std::malloc(blockSize * capacity);

	//Generate free blocks
	for (std::size_t i = 0; i < capacity; i++)
		mFreeBlocks[i] = static_cast<uint8_t*>(mData) + (i * mBlockSize);
	LOG("allocated %zu blocks, block size %zu", capacity,blockSize);
}

BlockAllocator::~BlockAllocator()
{
	// TODO
	ASSERT(mFreeBlocks.size() == mCapacity, "BlockAllocator -- Not all blocks are freed, Potential memory leak!");
	std::free(mData);

	LOG("destructed. Allocated %zu, Freed: %zu  High Water Mark: %zu", mBlocksAllocatedCurrent, mBlocksFreed,mBlocksHighest);
}

void* BlockAllocator::Allocate()
{
	// TODO
	if(mFreeBlocks.empty())
	return nullptr;

	//Get the next free block
	void* freeBlock = mFreeBlocks.back();
	mFreeBlocks.pop_back();

	++mBlocksAllocatedTotal;
	++mBlocksAllocatedCurrent;
	mBlocksHighest = std::max(mBlocksHighest,mBlocksAllocatedCurrent);

	LOG("allocated blocks at %p, Allocated: %zu, High Water Mark: %zu", freeBlock, mBlocksAllocatedCurrent, mBlocksHighest);

	return freeBlock;
}

// blockSize = 4
// blockCount = 10
//
// .... [                                   ]
//      |    |    |    |               |
//      0    4    8    12     ...      36
//      ^
//      mData
void BlockAllocator::Free(void* ptr)
{
	// TODO
	if (ptr == nullptr)
		return;

	const auto start = static_cast<uint8_t*>(mData);
	const auto end = static_cast<uint8_t*>(mData) + (mBlockSize * mCapacity);
	const auto current = static_cast<uint8_t*>(ptr);
	const auto diff = current - start;
	ASSERT(
		current >= start &&
		current < end&&
		static_cast<std::size_t>(diff) % mBlockSize == 0,
		"BlockAllocator -- Invalid address being freed.");

	LOG("free %p", ptr);
	--mBlocksAllocatedCurrent;
	++mBlocksFreed;
	mFreeBlocks.emplace_back(ptr);
}