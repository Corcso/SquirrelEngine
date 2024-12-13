#include "PCH.h"
#include "PoolAllocationService.h"

namespace SQ {
	void PoolAllocationService::Init()
	{
		for (int i = 0; i < 8; ++i) {
			pools[POOL_BLOCK_SIZES[i]] = std::unique_ptr<PoolAllocator>(new PoolAllocator(POOL_BLOCK_SIZES[i], POOL_BLOCK_COUNTS[i]));
		}
	}
}