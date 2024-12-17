#pragma once
#include "PCH.h"
#include "PoolAllocator.h"
#include <unordered_map>

#ifdef _DEBUG
#define POOL_ALLOCATOR_WARNINGS
#endif // _DEBUG

#include <iostream>
namespace SQ {
	constexpr int ALLOC_SERVICE_POOL_COUNT = 4;

	class PoolAllocationService
	{
	public:
		PoolAllocationService() {}

		void Init();


		

		/// <summary>
		/// Makes a unique pool pointer in the correct sized pool. 
		/// Otherwise falls back and uses new if pools are full. 
		/// </summary>
		/// <typeparam name="T">Type to create</typeparam>
		/// <typeparam name="...A">Constructor args</typeparam>
		/// <param name="...args">Constructor args</param>
		/// <returns>UniquePoolPtr to new object</returns>
		template <typename T, typename... A>
		inline UniquePoolPtr<T> MakeUniquePoolPtr(A... args) {
			// Round size up to pool size it will fit in
			size_t roundedSize = 0;
			for (int i = 0; i < ALLOC_SERVICE_POOL_COUNT; ++i) {
				if (sizeof(T) <= POOL_BLOCK_SIZES[i]) {
					roundedSize = POOL_BLOCK_SIZES[i];
					break;
				}
			}
			
			// If there isnt a fit use new as a fallback and output a warning
			if (roundedSize == 0) {
#ifdef POOL_ALLOCATOR_WARNINGS
				std::cout << "WARN : " << "Pool for size " << std::to_string(sizeof(T)) << " not available, new called.\n";
#endif // POOL_ALLOCATOR_WARNINGS
				return UniquePoolPtr<T>(new T(std::forward<A>(args)...));
			}

			// Check we have a pool which fits the size
			if (pools.find(roundedSize) != pools.end()) {
				PoolAllocator* poolToAllocInto = nullptr;
				if (pools[roundedSize]->GetBlocksInUse() < pools[roundedSize]->GetBlockCount()) {
					poolToAllocInto = pools[roundedSize].get();
				}
				if (poolToAllocInto != nullptr) return UniquePoolPtr<T>(poolToAllocInto->New<T>(std::forward<A>(args)...), poolToAllocInto);
				else {
#ifdef POOL_ALLOCATOR_WARNINGS
					std::cout << "WARN : " << "Pool for size " << std::to_string(roundedSize) << " full, new called.\n";
#endif // POOL_ALLOCATOR_WARNINGS
					return UniquePoolPtr<T>(new T(std::forward<A>(args)...));
				}
			}
			else {
				return UniquePoolPtr<T>(new T(std::forward<A>(args)...));
			}

			// We shouldn't reach here, throw an error. 
			throw 19;
			return UniquePoolPtr<T>();
		}

		/// <summary>
		/// Get a string with details of pools capacity. Useful for debug.
		/// </summary>
		/// <returns>Details of pools capacity</returns>
		std::string GetDebugPoolUsageStatement();

	private:
		// Sizes of pools to use and how many blocks to create
		// POOL SIZE MUST BE EQUAL OR MORE THAN 8 AND DIVISIBLE BY 8
		const size_t POOL_BLOCK_SIZES[ALLOC_SERVICE_POOL_COUNT] = { 128, 256, 512, 1024 };
		const size_t POOL_BLOCK_COUNTS[ALLOC_SERVICE_POOL_COUNT] = { 64, 128, 128, 64};

		std::unordered_map<size_t, std::unique_ptr<PoolAllocator>> pools;
	};
}