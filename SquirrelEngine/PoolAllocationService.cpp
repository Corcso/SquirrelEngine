#include "PCH.h"
#include "PoolAllocationService.h"

namespace SQ {
	void PoolAllocationService::Init()
	{
		// Create all pools
		for (int i = 0; i < ALLOC_SERVICE_POOL_COUNT; ++i) {
			pools[POOL_BLOCK_SIZES[i]] = std::unique_ptr<PoolAllocator>(new PoolAllocator(POOL_BLOCK_SIZES[i], POOL_BLOCK_COUNTS[i]));
		}
	}
	std::string PoolAllocationService::GetDebugPoolUsageStatement()
	{
		// Get useage details in output string nicely formatted. 
		// Includes blocks used and bytes used totals. 
		std::string output = "";
		unsigned int totalPoolSize = 0;
		unsigned int totalPoolSizeInUse = 0;
		for (int i = 0; i < ALLOC_SERVICE_POOL_COUNT; ++i) {
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

	void PoolAllocationService::ImGuiPoolUsageRender()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Small Pool Allocators"))
		{
			unsigned int totalPoolSize = 0;
			unsigned int totalPoolSizeInUse = 0;
			std::string statementBar = "";
			for (int i = 0; i < ALLOC_SERVICE_POOL_COUNT; ++i) {
				ImGui::Text(("Pool Size " + std::to_string(POOL_BLOCK_SIZES[i])).c_str());
				statementBar = std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse()) + "/" + std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlockCount());
				ImGui::ProgressBar((float)pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse() / (float)pools[POOL_BLOCK_SIZES[i]]->GetBlockCount(), ImVec2(0.0f, 0.0f), statementBar.c_str());
				statementBar = std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse() * POOL_BLOCK_SIZES[i]) + "B/" + std::to_string(pools[POOL_BLOCK_SIZES[i]]->GetBlockCount() * POOL_BLOCK_SIZES[i]) + "B";
				unsigned int poolSize = pools[POOL_BLOCK_SIZES[i]]->GetBlockCount() * POOL_BLOCK_SIZES[i];
				unsigned int poolSizeInUse = pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse() * POOL_BLOCK_SIZES[i];
				ImGui::ProgressBar(
					(float)poolSizeInUse / (float)poolSize,
					ImVec2(0.0f, 0.0f), statementBar.c_str());
				totalPoolSize += pools[POOL_BLOCK_SIZES[i]]->GetBlockCount() * POOL_BLOCK_SIZES[i];
				totalPoolSizeInUse += pools[POOL_BLOCK_SIZES[i]]->GetBlocksInUse() * POOL_BLOCK_SIZES[i];
			}
			ImGui::Text("Total");
			statementBar = std::to_string(totalPoolSizeInUse) + "B/" + std::to_string(totalPoolSize) + "B";
			ImGui::ProgressBar((float)totalPoolSizeInUse / (float)totalPoolSize, ImVec2(0.0f, 0.0f), statementBar.c_str());
			
			ImGui::TreePop();
		}
	}
}