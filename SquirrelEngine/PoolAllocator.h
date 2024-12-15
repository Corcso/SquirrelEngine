#pragma once

// Thread safe includes
#include <mutex>
#include <atomic>
namespace SQ {
	/// <summary>
	/// Thead safe pool allocator (Gregory, 2018)
	/// </summary>
	class PoolAllocator
	{
	public:
		/// <summary>
		/// Constructor for pool allocator, allocates all memory for the pool
		/// </summary>
		/// <param name="blockSize">Size of a block</param>
		/// <param name="blockCount">Number of blocks in the pool</param>
		PoolAllocator(size_t blockSize, unsigned int blockCount) {
			// TODO error if block size < size of void*
			this->blockSize = blockSize;
			this->blockCount = blockCount;
			blocksInUse = 0;

			base = calloc(blockCount, blockSize);
			char* forAddition = reinterpret_cast<char*>(base);
			nextFreeLocation = base;
			// Push locations onto free locations available in reverse order. 
			/*for (int b = blockCount - 1; b >= 0; --b) {
				freeLocations.push_front(reinterpret_cast<void*>(forAddition + (blockSize * b)));
			}*/
			for (unsigned int b = 0; b < blockCount - 1; ++b) {
				*reinterpret_cast<void**>(forAddition + (blockSize * b)) = reinterpret_cast<void*>(forAddition + (blockSize * (b + 1)));
			}
			*reinterpret_cast<void**>(forAddition + (blockSize * (blockCount - 1))) = nullptr;
		}

		/// <summary>
		/// Allocate memory on the pool
		/// </summary>
		/// <returns>Pointer to free memory, blockSize big</returns>
		void* Alloc() {
			// TODO throw if alloc over bounds
			std::lock_guard<std::mutex> poolLock(poolMutex);
			void* freeAddress = nextFreeLocation;
			nextFreeLocation = *reinterpret_cast<void**>(freeAddress);
			++blocksInUse;
			return freeAddress;
		}

		/// <summary>
		/// <para>Frees memory on the pool</para>
		/// <para>This block becomes the next chosen one</para>
		/// </summary>
		/// <param name="address">Address of pool block to free</param>
		void Free(void* address) {
			std::lock_guard<std::mutex> poolLock(poolMutex);
			*reinterpret_cast<void**>(address) = nextFreeLocation;
			nextFreeLocation = address;
			--blocksInUse;
			return;
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
			// TODO throw if new over bounds
			std::lock_guard<std::mutex> poolLock(poolMutex);
			void* freeAddress = nextFreeLocation;
			nextFreeLocation = *reinterpret_cast<void**>(freeAddress);
			T* toReturn = new(freeAddress) T(std::forward<A>(args)...);
			++blocksInUse;
			return toReturn;
		}

		/// <summary>
		/// Simulates delete keyword, calls deconstructor
		/// </summary>
		/// <typeparam name="T">Type being deleted</typeparam>
		/// <param name="address">Address of data to delete</param>
		template <typename T>
		inline void Delete(void* address) {
			reinterpret_cast<T*>(address)->~T();
			std::lock_guard<std::mutex> poolLock(poolMutex);
			*reinterpret_cast<void**>(address) = nextFreeLocation;
			nextFreeLocation = address;
			--blocksInUse;
			return;
		}

		unsigned int GetBlocksInUse() {
			return blocksInUse;
		}

		unsigned int GetBlockCount() {
			return blockCount;
		}

		~PoolAllocator() {
			free(base);
		}
	private:
		/// <summary>
		/// Mutex for pool access for thread saftey
		/// </summary>
		std::mutex poolMutex;

		/// <summary>
		/// Pointer to the base
		/// </summary>
		void* base;
		/// <summary>
		/// Size of each block
		/// </summary>
		size_t blockSize;
		/// <summary>
		/// Count of blocks
		/// </summary>
		unsigned int blockCount;
		/// <summary>
		/// Number of blocks in use
		/// </summary>
		unsigned int blocksInUse;

		/// <summary>
		/// Pointer to the next free location. 
		/// <para>Head of the linked list of free locations</para>
		/// <para>Each free block stores a void* to next free location.</para>
		/// </summary>
		void* nextFreeLocation;
	};

	/// <summary>
	/// Unique pointer for use with PoolAllocator
	/// </summary>
	/// <typeparam name="T">Type of object</typeparam>
	template<typename T>
	class UniquePoolPtr {
	public:
		inline UniquePoolPtr<T>() {
			//By default, our underlying pointer should be empty
			rawPointer = nullptr;
			poolAllocator = nullptr;
		}

		inline UniquePoolPtr<T>(T* p, PoolAllocator* pp) {
			//By default, our underlying pointer should be empty
			rawPointer = p;
			poolAllocator = pp;
		}

		inline UniquePoolPtr<T>(T* p) {
			//By default, our underlying pointer should be empty
			rawPointer = p;
			poolAllocator = nullptr;
		}

		// No Copy Allowed
		inline UniquePoolPtr<T>(const UniquePoolPtr<T>&) = delete;
		UniquePoolPtr<T>& operator=(const UniquePoolPtr<T>&) = delete;

		// Move Allowed
		inline UniquePoolPtr<T>(UniquePoolPtr<T>&& toMove) noexcept {
			rawPointer = toMove.rawPointer;
			poolAllocator = toMove.poolAllocator;
			toMove.rawPointer = nullptr;
			toMove.poolAllocator = nullptr;
		}

		UniquePoolPtr<T>& operator=(UniquePoolPtr<T>&& toMove) noexcept {
			if (this == &toMove) *this;
			rawPointer = toMove.rawPointer;
			poolAllocator = toMove.poolAllocator;
			toMove.rawPointer = nullptr;
			toMove.poolAllocator = nullptr;
			return *this;
		};

		template <typename TO>
		inline UniquePoolPtr<TO> DynamicUniquePoolPtrCast() {
			UniquePoolPtr<TO> result(static_cast<TO*>(rawPointer), poolAllocator);
			if (dynamic_cast<TO*>(rawPointer) != nullptr) {
				rawPointer = nullptr;
				poolAllocator = nullptr;
			}
			return std::move(result);
		}

		template <typename TO>
		inline UniquePoolPtr<TO> StaticUniquePoolPtrCast() {
			UniquePoolPtr<TO> result(static_cast<TO*>(rawPointer), poolAllocator);
			if (static_cast<TO*>(rawPointer) != nullptr) {
				rawPointer = nullptr;
				poolAllocator = nullptr;
			}
			return std::move(result);
		}

		// Data operations
		inline void release() {
			rawPointer = nullptr;
			poolAllocator = nullptr;
		}

		inline T* get() {
			return rawPointer;
		}

		inline T* operator->(/*const SmartPointer<T>&*/) {
			return rawPointer;
		}

		inline T& operator*(/*const SmartPointer<T>&*/) {
			return *rawPointer;
		}

		~UniquePoolPtr<T>() {
			if (poolAllocator && rawPointer) poolAllocator->Delete<T>(rawPointer);
			if (!poolAllocator && rawPointer) delete rawPointer;
		}

	private:
		// Pool allocator which holds the pointer
		PoolAllocator* poolAllocator;
		//The underlying pointer
		T* rawPointer;

	};

	/// <summary>
	/// Makes a unique pool ptr, calls new T
	/// </summary>
	/// <typeparam name="T">Type of object</typeparam>
	/// <typeparam name="...A">Constructor Args</typeparam>
	/// <param name="pool">Pool to place object in</param>
	/// <param name="...args">Constructor Args</param>
	/// <returns>UniquePoolPtr of object</returns>
	template <typename T, typename... A>
	inline UniquePoolPtr<T> MakeUniquePoolPtr(PoolAllocator* pool, A... args) {
		return UniquePoolPtr<T>(pool->New<T>(std::forward<A>(args)...), pool);
	}

	template<typename T>
	class WeakPoolPtr;
	template<typename T>
	class SharedPoolPtr;

	/// <summary>
	/// Shared pointer, for use with PoolAllocator
	/// </summary>
	/// <typeparam name="T">Type of Object</typeparam>
	template<typename T>
	class SharedPoolPtr {

	public:
		friend class WeakPoolPtr<T>;
		inline SharedPoolPtr<T>() {
			//By default, our underlying pointer should be empty
			rawPointer = nullptr;
			poolAllocator = nullptr;
			referenceCount = nullptr;
			referenceCountIncWeak = nullptr;
		}

		inline SharedPoolPtr<T>(T* p, PoolAllocator* pp) {
			//By default, our underlying pointer should be empty
			rawPointer = p;
			poolAllocator = pp;
			referenceCount = new std::atomic<unsigned int>(1);
			referenceCountIncWeak = new std::atomic<unsigned int>(1);
		}

		// Copy Allowed
		inline SharedPoolPtr<T>(const SharedPoolPtr<T>& toCopy) {
			rawPointer = toCopy.rawPointer;
			poolAllocator = toCopy.poolAllocator;
			referenceCount = toCopy.referenceCount;
			referenceCountIncWeak = toCopy.referenceCountIncWeak;

			if (referenceCount != nullptr) ++(*referenceCount);
			if (referenceCountIncWeak != nullptr) ++(*referenceCountIncWeak);
		};
		SharedPoolPtr<T>& operator=(const SharedPoolPtr<T>& toCopy) {
			// Return self if copying to self
			if (this == &toCopy)  *this;

			// We are about to be overwritten and therefor loose our reference to what we were originally pointing too (if anything)
			// Take one off our ref count and check for deletions. 
			if (referenceCount != nullptr) --(*referenceCount);
			if (referenceCountIncWeak != nullptr) --(*referenceCountIncWeak);
			CheckClear();

			rawPointer = toCopy.rawPointer;
			poolAllocator = toCopy.poolAllocator;
			referenceCount = toCopy.referenceCount;
			referenceCountIncWeak = toCopy.referenceCountIncWeak;

			if (referenceCount != nullptr) ++(*referenceCount);
			if (referenceCountIncWeak != nullptr) ++(*referenceCountIncWeak);

			return *this;
		};;

		// Move Allowed
		inline SharedPoolPtr<T>(UniquePoolPtr<T>&& toMove) noexcept {
			rawPointer = toMove.rawPointer;
			poolAllocator = toMove.poolAllocator;
			referenceCount = toMove.referenceCount;
			referenceCountIncWeak = toMove.referenceCountIncWeak;
			toMove.rawPointer = nullptr;
			toMove.poolAllocator = nullptr;
			toMove.referenceCount = nullptr;
			toMove.referenceCountIncWeak = nullptr;
		}
		SharedPoolPtr<T>& operator=(SharedPoolPtr<T>&& toMove) noexcept {
			if (this == &toMove) *this;

			// We are about to be overwritten and therefor loose our reference to what we were originally pointing too (if anything)
			// Take one off our ref count and check for deletions. 
			if (referenceCount != nullptr) --(*referenceCount);
			if (referenceCountIncWeak != nullptr) --(*referenceCountIncWeak);
			CheckClear();

			rawPointer = toMove.rawPointer;
			poolAllocator = toMove.poolAllocator;
			referenceCount = toMove.referenceCount;
			referenceCountIncWeak = toMove.referenceCountIncWeak;
			toMove.rawPointer = nullptr;
			toMove.poolAllocator = nullptr;
			toMove.referenceCount = nullptr;
			toMove.referenceCountIncWeak = nullptr;
			return *this;
		};

		inline unsigned int GetRefCount() {
			if (referenceCount != nullptr) return *referenceCount;
			else return -1;
		}

		inline unsigned int GetRefCountIncWeak() {
			if (referenceCountIncWeak != nullptr) return *referenceCountIncWeak;
			else return -1;
		}

		// Data operations
		inline T* get() {
			return rawPointer;
		}

		inline T* operator->(/*const SmartPointer<T>&*/) {
			return rawPointer;
		}

		inline T& operator*(/*const SmartPointer<T>&*/) {
			return *rawPointer;
		}

		~SharedPoolPtr<T>() {
			if (referenceCount != nullptr) --(*referenceCount);
			if (referenceCountIncWeak != nullptr) --(*referenceCountIncWeak);
			CheckClear();
		}

	private:


		// For weak pointer lock to use
		inline SharedPoolPtr<T>(const WeakPoolPtr<T>& toCopy) {
			//By default, our underlying pointer should be empty
			rawPointer = toCopy.rawPointer;
			poolAllocator = toCopy.poolAllocator;
			referenceCount = toCopy.referenceCount;
			referenceCountIncWeak = toCopy.referenceCountIncWeak;

			++(*referenceCount);
			++(*referenceCountIncWeak);
		}

		inline void CheckClear() {
			if (referenceCount != nullptr && *referenceCount <= 0) {
				poolAllocator->Delete<T>(rawPointer);
			}
			if (referenceCountIncWeak != nullptr && *referenceCountIncWeak <= 0) {
				delete referenceCount;
				delete referenceCountIncWeak;
			}
		}

		// Pool allocator which holds the pointer
		PoolAllocator* poolAllocator;
		//The underlying pointer
		T* rawPointer;
		std::atomic<unsigned int>* referenceCount;
		std::atomic<unsigned int>* referenceCountIncWeak;

	};

	/// <summary>
	/// Makes a shared pool ptr, calls new T
	/// </summary>
	/// <typeparam name="T">Type of object</typeparam>
	/// <typeparam name="...A">Constructor Args</typeparam>
	/// <param name="pool">Pool to place object in</param>
	/// <param name="...args">Constructor Args</param>
	/// <returns>SharedPoolPtr of object</returns>
	template <typename T, typename... A>
	inline SharedPoolPtr<T> MakeSharedPoolPtr(PoolAllocator* pool, A... args) {
		return SharedPoolPtr<T>(pool->New<T>(std::forward<A>(args)...), pool);
	}

	/// <summary>
	/// Weak pointer for use with PoolAllocator
	/// </summary>
	/// <typeparam name="T">Type of object</typeparam>
	template<typename T>
	class WeakPoolPtr {
	public:
		friend class SharedPoolPtr<T>;

		inline WeakPoolPtr<T>() {
			//By default, our underlying pointer should be empty
			rawPointer = nullptr;
			poolAllocator = nullptr;
			referenceCount = nullptr;
			referenceCountIncWeak = nullptr;
		}

		inline WeakPoolPtr<T>(const SharedPoolPtr<T>& toCopy) {
			//By default, our underlying pointer should be empty
			rawPointer = toCopy.rawPointer;
			poolAllocator = toCopy.poolAllocator;
			referenceCount = toCopy.referenceCount;
			referenceCountIncWeak = toCopy.referenceCountIncWeak;

			if (referenceCountIncWeak != nullptr) ++(*referenceCountIncWeak);
		}

		// Copy Allowed
		inline WeakPoolPtr<T>(const WeakPoolPtr<T>& toCopy) {
			rawPointer = toCopy.rawPointer;
			poolAllocator = toCopy.poolAllocator;
			referenceCount = toCopy.referenceCount;
			referenceCountIncWeak = toCopy.referenceCountIncWeak;

			if (referenceCountIncWeak != nullptr) ++(*referenceCountIncWeak);
		};
		WeakPoolPtr<T>& operator=(const WeakPoolPtr<T>& toCopy) {
			// Return self if copying to self
			if (this == &toCopy)  *this;

			if (referenceCountIncWeak != nullptr) --(*referenceCountIncWeak);
			CheckClear();

			rawPointer = toCopy.rawPointer;
			poolAllocator = toCopy.poolAllocator;
			referenceCount = toCopy.referenceCount;
			referenceCountIncWeak = toCopy.referenceCountIncWeak;

			if (referenceCountIncWeak != nullptr) ++(*referenceCountIncWeak);

			return *this;
		};;

		// Move Allowed
		inline WeakPoolPtr<T>(UniquePoolPtr<T>&& toMove) noexcept {
			rawPointer = toMove.rawPointer;
			poolAllocator = toMove.poolAllocator;
			referenceCount = toMove.referenceCount;
			referenceCountIncWeak = toMove.referenceCountIncWeak;
			toMove.rawPointer = nullptr;
			toMove.poolAllocator = nullptr;
			toMove.referenceCount = nullptr;
			toMove.referenceCountIncWeak = nullptr;
		}
		WeakPoolPtr<T>& operator=(WeakPoolPtr<T>&& toMove) noexcept {
			if (this == &toMove) *this;

			if (referenceCountIncWeak != nullptr) --(*referenceCountIncWeak);
			CheckClear();

			rawPointer = toMove.rawPointer;
			poolAllocator = toMove.poolAllocator;
			referenceCount = toMove.referenceCount;
			referenceCountIncWeak = toMove.referenceCountIncWeak;
			toMove.rawPointer = nullptr;
			toMove.poolAllocator = nullptr;
			toMove.referenceCount = nullptr;
			toMove.referenceCountIncWeak = nullptr;
			return *this;
		};

		// Expired function
		bool Expired() {
			return (*referenceCount <= 0);
		}

		// Lock function
		SharedPoolPtr<T> Lock() {
			return SharedPoolPtr<T>(*this);
		}

		// Data operations
		inline T* get() {
			return rawPointer;
		}

		inline T* operator->(/*const SmartPointer<T>&*/) {
			return rawPointer;
		}

		inline T& operator*(/*const SmartPointer<T>&*/) {
			return *rawPointer;
		}

		~WeakPoolPtr<T>() {
			if (referenceCountIncWeak != nullptr) --(*referenceCountIncWeak);
			CheckClear();
		}

	private:

		inline void CheckClear() {
			if (referenceCountIncWeak != nullptr && *referenceCountIncWeak <= 0) {
				delete referenceCount;
				delete referenceCountIncWeak;
			}
		}

		// Pool allocator which holds the pointer
		PoolAllocator* poolAllocator;
		//The underlying pointer
		T* rawPointer;
		std::atomic<unsigned int>* referenceCount;
		std::atomic<unsigned int>* referenceCountIncWeak;

	};
}