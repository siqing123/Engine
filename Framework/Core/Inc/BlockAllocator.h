#pragma once

namespace Storm::Core
{
	// Description: This is a basic block allocator which performs a single
	// allocation and deallocation on construction/destruction. Blocks are
	// assigned and recycled to the user.
	//
	// Usage:
	//		BlockAllocator blockAllocator(10, 10);
	//		void* memory = blockAllocator.Allocate();
	//		....
	//		blockAllocator.Free(memory);
	//
	class BlockAllocator
	{
	public:
		BlockAllocator(size_t blockSize, size_t capacity); // Allocate all memory here
		virtual ~BlockAllocator();

		// No copy/move/assignment
		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;
		BlockAllocator(BlockAllocator&&) = delete;
		BlockAllocator& operator=(BlockAllocator&&) = delete;

		void* Allocate();
		void Free(void* ptr);

	private:
		std::string mName;
		std::vector<void*> mFreeBlocks;
		void* mData = nullptr;
		//void* mStart = nullptr;
		//void* mEnd = nullptr;
		size_t mBlockSize = 0;	// How many bytes for a single block
		size_t mCapacity = 0;	// How many blocks
		size_t mBlocksAllocatedCurrent = 0;
		size_t mBlocksAllocatedTotal = 0;
		size_t mBlocksFreed = 0;
		size_t mBlocksHighest = 0;
	};
}

#define MEMORY_POOL_DECLARE\
	static Storm::Core::BlockAllocator sAllocator;\
	static void* operator new(std::size_t size);\
	static void operator delete(void* ptr);

#define MEMORY_POOL_DEFINE(Class, Capacity)\
	Storm::Core::BlockAllocator Class::sAllocator(sizeof(Class),Capacity);\
	void* Class::operator new(std::size_t size) {return sAllocator.Allocate();}\
	void Class::operator delete(void* ptr) {sAllocator.Free(ptr);}