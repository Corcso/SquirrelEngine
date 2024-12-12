#pragma once
#include <forward_list>
namespace SQ {
	class PoolAllocator
	{
	public:
		PoolAllocator(size_t blockSize, unsigned int blockCount) {
			this->blockSize = blockSize;
			this->blockCount = blockCount;
			blocksInUse = 0;

			base = malloc(blockSize * blockCount);
			char* forAddition = reinterpret_cast<char*>(base);
			// Push locations onto free locations available in reverse order. 
			for (int b = blockCount - 1; b >= 0; --b) {
				freeLocations.push_front(reinterpret_cast<void*>(forAddition + (blockSize * b)));
			}
		}

		void* Alloc() {
			void* freeAddress = freeLocations.front();
			freeLocations.pop_front();
			++blocksInUse;
			return freeAddress;
		}

		void Free(void* address) {
			freeLocations.push_front(address);
			--blocksInUse;
			return;
		}

		template <typename T, typename... A>
		inline T* New(A... args) {
			void* freeAddress = freeLocations.front();
			freeLocations.pop_front();
			T* toReturn = new(freeAddress) T(std::forward<A>(args)...);
			++blocksInUse;
			return toReturn;
		}

		template <typename T>
		inline void Delete(void* address) {
			freeLocations.push_front(address);
			reinterpret_cast<T*>(address)->~T();
			--blocksInUse;
			return;
		}

	private:
		void* base;
		size_t blockSize;
		unsigned int blockCount;
		unsigned int blocksInUse;

		std::forward_list<void*> freeLocations;
	};
}