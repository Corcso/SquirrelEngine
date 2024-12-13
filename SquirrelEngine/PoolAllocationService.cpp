#include "PCH.h"
#include "PoolAllocationService.h"

namespace SQ {
	void PoolAllocationService::Init()
	{
		for (int i = 0; i < 8; ++i) {
			pools[POOL_BLOCK_SIZES[i]] = std::unique_ptr<PoolAllocator>(new PoolAllocator(POOL_BLOCK_SIZES[i], POOL_BLOCK_COUNTS[i]));
		}
	}
	std::string PoolAllocationService::GetDebugPoolUsageStatement()
	{
		std::string output = "";
		unsigned int totalPoolSize = 0;
		unsigned int totalPoolSizeInUse = 0;
		for (int i = 0; i < 8; ++i) {
			output += "Pool Size " + std::to_string(POOL_BLOCK_SIZES[i]) + " at ";
			output += std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse()) + "/" + std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlockCount()) + " | ";
			output += std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse() * POOL_BLOCK_SIZES[i]) + "B/" + std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlockCount() * POOL_BLOCK_SIZES[i]) + "B\n";
			totalPoolSize += pools[POOL_BLOCK_SIZES[i]]->GetBlockCount() * POOL_BLOCK_SIZES[i];
			totalPoolSizeInUse += pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse() * POOL_BLOCK_SIZES[i];
		}
		output += "Total Pool Space In Use: " + std::to_string(totalPoolSizeInUse) + " Bytes\n"
			+ "Total Pool Space Free: " + std::to_string(totalPoolSize - totalPoolSizeInUse) + " Bytes\n"
			+ "Total Pool Space: " + std::to_string(totalPoolSize) + " Bytes\n";
		return output;
	}
}