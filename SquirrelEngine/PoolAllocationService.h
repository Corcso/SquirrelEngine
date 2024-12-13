#pragma once
#include "PCH.h"
#include "PoolAllocator.h"
#include <unordered_map>
namespace SQ {
	class PoolAllocationService
	{
	public:
		PoolAllocationService() {}

		void Init();


		//void* Alloc(size_t size) {
		//	// Round size up to pool size it will fit in
		//	size_t roundedSize = 0;
		//	for (int i = 0; i < 8; ++i) {
		//		if (size <= POOL_BLOCK_SIZES[i]) {
		//			roundedSize = POOL_BLOCK_SIZES[i];
		//			break;
		//		}
		//	}
		//	
		//	// If there isnt a fit
		//	// TODO POOL NOT BIG ENOUGH
		//	if (roundedSize == 0);

		//	// Check we have a pool which fits the size
		//	if (pools.find(roundedSize) != pools.end()) {
		//		PoolAllocator* poolToAllocInto = nullptr;
		//		for (int vp = 0; vp < pools[roundedSize].size(); ++vp) {
		//			if (pools[roundedSize][vp]->GetBlocksInUse() < pools[roundedSize][vp]->GetBlockCount()) {
		//				poolToAllocInto = pools[roundedSize][vp].get();
		//			}
		//		}
		//		if (poolToAllocInto == nullptr) {
		//			pools[roundedSize].push_back(std::make_unique<PoolAllocator>(roundedSize, 64));
		//			poolToAllocInto = pools[roundedSize].back().get();
		//		}

		//		return poolToAllocInto->Alloc();
		//	}
		//	else; //TODO THROW ERROR
		//}

		/*/// <summary>
		/// <para>Frees memory on the pool</para>
		/// <para>This block becomes the next chosen one</para>
		/// </summary>
		/// <param name="address">Address of pool block to free</param>
		void Free(void* address) {

		}

		/// <summary>
		/// Simulates new keyword, but for allocation on the pool
		/// </summary>
		/// <typeparam name="T">Type to create</typeparam>
		/// <typeparam name="...A">Constructor Args</typeparam>
		/// <param name="...args">Constructor Args</param>
		/// <returns>Pointer to new object on the pool</returns>
		template <typename T, typename... A>
		inline T* New(A... args) {

		}

		/// <summary>
		/// Simulates delete keyword, calls deconstructor
		/// </summary>
		/// <typeparam name="T">Type being deleted</typeparam>
		/// <param name="address">Address of data to delete</param>
		template <typename T>
		inline void Delete(void* address) {

		}*/

		//template <typename T, typename... A>
		//inline UniquePoolPtr<T> MakeUniquePoolPtr(A... args) {
		//	// Round size up to pool size it will fit in
		//	size_t roundedSize = 0;
		//	for (int i = 0; i < 8; ++i) {
		//		if (sizeof(T) <= POOL_BLOCK_SIZES[i]) {
		//			roundedSize = POOL_BLOCK_SIZES[i];
		//			break;
		//		}
		//	}
		//	
		//	// If there isnt a fit
		//	// TODO POOL NOT BIG ENOUGH
		//	if (roundedSize == 0);

		//	// Check we have a pool which fits the size
		//	if (pools.find(roundedSize) != pools.end()) {
		//		PoolAllocator* poolToAllocInto = nullptr;
		//		for (int vp = 0; vp < pools[roundedSize].size(); ++vp) {
		//			if (pools[roundedSize][vp]->GetBlocksInUse() < pools[roundedSize][vp]->GetBlockCount()) {
		//				poolToAllocInto = pools[roundedSize][vp].get();
		//			}
		//		}
		//		if (poolToAllocInto == nullptr) {
		//			pools[roundedSize].push_back(std::make_unique<PoolAllocator>(roundedSize, 64));
		//			poolToAllocInto = pools[roundedSize].back().get();
		//		}
		//		return UniquePoolPtr<T>(poolToAllocInto->New<T>(std::forward<A>(args)...), poolToAllocInto)
		//	}
		//	else; //TODO THROW ERROR


		//	return UniquePoolPtr<T>();
		//}

		template <typename T, typename... A>
		inline UniquePoolPtr<T> MakeUniquePoolPtr(A... args) {
			// Round size up to pool size it will fit in
			size_t roundedSize = 0;
			for (int i = 0; i < 8; ++i) {
				if (sizeof(T) <= POOL_BLOCK_SIZES[i]) {
					roundedSize = POOL_BLOCK_SIZES[i];
					break;
				}
			}
			
			// If there isnt a fit
			// TODO POOL NOT BIG ENOUGH
			if (roundedSize == 0) throw 99;

			// Check we have a pool which fits the size
			if (pools.find(roundedSize) != pools.end()) {
				PoolAllocator* poolToAllocInto = nullptr;
				if (pools[roundedSize]->GetBlocksInUse() < pools[roundedSize]->GetBlockCount()) {
					poolToAllocInto = pools[roundedSize].get();
				}
				if (poolToAllocInto != nullptr) return UniquePoolPtr<T>(poolToAllocInto->New<T>(std::forward<A>(args)...), poolToAllocInto);
			}
			else throw 100; //TODO THROW ERROR

			throw 101;
			return UniquePoolPtr<T>();
		}

		std::string GetDebugPoolUsageStatement();

	private:
		const size_t POOL_BLOCK_SIZES[8] = { 16, 32, 64, 128, 256, 512, 1024, 2048 };
		const size_t POOL_BLOCK_COUNTS[8] = { 64, 64, 64, 32, 32, 16, 16, 16 };

		std::unordered_map<size_t, std::unique_ptr<PoolAllocator>> pools;
	};
}