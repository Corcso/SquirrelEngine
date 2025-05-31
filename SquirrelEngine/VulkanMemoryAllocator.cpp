#include "PCH.h"
#ifdef VULKAN

#include "VulkanMemoryAllocator.h"
#include "VulkanUtility.h"

#include <iostream>
namespace SQ {
    VulkanMemoryAllocator::VulkanMemoryBlock SQ::VulkanMemoryAllocator::BindBufferToMemory(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties, bool isMapInstantCopy, VkBuffer toBind)
    {
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, toBind, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VulkanUtility::FindMemoryTypeIndex(physicalDevice, memRequirements.memoryTypeBits, properties);

        VulkanMemoryBlock block = FindMemoryBlock(device, allocInfo, isMapInstantCopy);

        vkBindBufferMemory(device, toBind, memoryPools[block.poolID][block.location.poolIndex], block.location.offset);

        return block;
    }

    void VulkanMemoryAllocator::FreeMemory(VkDevice device, VulkanMemoryBlock block)
    {
        // If our block size is standard treat it differerantly 
        // TEMP
        // TODO drop all empty pools
        if (sizeToBlockCountPerAlloc.find(block.poolID.blockSize) != sizeToBlockCountPerAlloc.end()) {
            // Just mark as free, don't drop any pools. Need to drop a pool when its completely empty but will require a full linked list traversal at the moment I think.
            freeMemoryLocations[block.poolID].push_front(block.location);
        }
        else {
            // Drop the custom size pool
            vkFreeMemory(device, memoryPools[block.poolID][block.location.poolIndex], nullptr);
            memoryPools[block.poolID].erase(memoryPools[block.poolID].begin() + block.location.poolIndex);
        }
    }

    VkDeviceMemory VulkanMemoryAllocator::GetBlockMemoryAllocation(VulkanMemoryAllocator::VulkanMemoryBlock block)
    {
        return memoryPools[block.poolID][block.location.poolIndex];
    }

    VulkanMemoryAllocator::VulkanMemoryBlock SQ::VulkanMemoryAllocator::FindMemoryBlock(VkDevice device, VkMemoryAllocateInfo desiredAllocation, bool isMapInstantCopy)
    {
        // Find block size
        uint32_t blockSize = 0;

        for (auto& pair : sizeToBlockCountPerAlloc) {
            if (desiredAllocation.allocationSize <= pair.first) {
                blockSize = pair.first;
                break;
            }
        }

        if (blockSize != 0) {
            // Create pool identifier
            VulkanMemoryPoolIdentifier poolID = { desiredAllocation.memoryTypeIndex, blockSize, isMapInstantCopy };
            // Look to see if pool exists
            if (memoryPools.find(poolID) != memoryPools.end() && memoryPools[poolID].size() != 0) {
                // Then check if there is space
                if (freeMemoryLocations[poolID].size() != 0) {
                    // Then return the free space
                    VulkanMemoryPoolLocation location = freeMemoryLocations[poolID].front();
                    freeMemoryLocations[poolID].pop_front();
                    std::cout << "Existing block taken, Memory Index:" << poolID.memoryTypeIndex << " Size: " << poolID.blockSize << "*" << sizeToBlockCountPerAlloc[poolID.blockSize] << " Index: " << location.poolIndex << " Offset: " << location.offset << "\n";
                    return { poolID, location };
                }
                //// If there isnt, make a new pool in the list (list already exists as there is at least 1 taken pool)
                //uint32_t newPoolIndex = memoryPools[poolID].size();
                //memoryPools[poolID].push_back(nullptr);
                //VkMemoryAllocateInfo allocInfo{};
                //allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
                //allocInfo.allocationSize = poolID.blockSize * sizeToBlockCountPerAlloc[poolID.blockSize];
                //allocInfo.memoryTypeIndex = poolID.memoryTypeIndex;
                //if (vkAllocateMemory(device, &allocInfo, nullptr, &(memoryPools[poolID][newPoolIndex])) != VK_SUCCESS) {
                //    throw - 1;
                //}
                //// Create free locations (appart from the first one, we are about to use it)
                //for (int i = 1; i < sizeToBlockCountPerAlloc[poolID.blockSize]; i++) {
                //    freeMemoryLocations[poolID].push_back({ newPoolIndex , i * blockSize});
                //}
                //// First block of the new pool is reserved
                //return { newPoolIndex, 0 };
            }
            // If the pool does not exist, create it. We dont care if its in the map, just implicitally create it if it isnt. 
            uint32_t newPoolIndex = memoryPools[poolID].size();
            memoryPools[poolID].push_back(nullptr);
            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = poolID.blockSize * sizeToBlockCountPerAlloc[poolID.blockSize];
            allocInfo.memoryTypeIndex = poolID.memoryTypeIndex;
            if (vkAllocateMemory(device, &allocInfo, nullptr, &(memoryPools[poolID][newPoolIndex])) != VK_SUCCESS) {
                throw - 1;
            }
            // Create free locations (appart from the first one, we are about to use it)
            for (int i = 1; i < sizeToBlockCountPerAlloc[poolID.blockSize]; i++) {
                freeMemoryLocations[poolID].push_back({ newPoolIndex , i * blockSize });
            }
            std::cout << "New pool made, Memory Index:" << poolID.memoryTypeIndex << " Size: " << poolID.blockSize << "*" << sizeToBlockCountPerAlloc[poolID.blockSize] << " Index: " << newPoolIndex << "\n";
            // First block of the new pool is reserved
            return { poolID, { newPoolIndex, 0 } };
        }
        // The block size we want, isnt in the list of sizes, create a 1 pool block for this as a last resort. 
        VulkanMemoryPoolIdentifier poolID = { desiredAllocation.memoryTypeIndex, desiredAllocation.allocationSize, isMapInstantCopy };
        // There is any chance a pool of this exact size also exists, so treat this as an extra pool
        uint32_t newPoolIndex = memoryPools[poolID].size();
        memoryPools[poolID].push_back(nullptr);
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = poolID.blockSize;
        allocInfo.memoryTypeIndex = poolID.memoryTypeIndex;
        if (vkAllocateMemory(device, &allocInfo, nullptr, &(memoryPools[poolID][newPoolIndex])) != VK_SUCCESS) {
            throw - 1;
        }
        // No free locations for this, were making a 1 block pool, just its own allocation. 
        // First block of the new pool is reserved
        return { poolID, { newPoolIndex, 0 } };
        std::cout << "New custom pool made, Memory Index:" << poolID.memoryTypeIndex << " Size: " << poolID.blockSize << " Index: " << newPoolIndex << "\n";
    }
}

#endif