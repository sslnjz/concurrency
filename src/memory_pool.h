#pragma once

#ifndef CONCURRENCY_MEMORY_POOL_H
#define CONCURRENCY_MEMORY_POOL_H

#include <cstddef>

namespace concurrent
{
	class memory_pool
	{
	private:
		unsigned int	_blocks_num;			// Num of blocks
		unsigned int	_block_size;			// Size of each block
		unsigned int	_free_blocks_num;		// Num of remaining blocks
		unsigned int	_initialized_num;		// Num of initialized blocks
		unsigned char*	_beginning_pool;		// Beginning of memory pool
		unsigned char*	_next_free_block;		// Num of next free block

	public:
		memory_pool()
		{
			_blocks_num = 0;
			_block_size = 0;
			_free_blocks_num = 0;
			_initialized_num = 0;
			_beginning_pool = nullptr;
			_next_free_block = 0;
		}
		~memory_pool() 
		{ 
			destroypool(); 
		}


		void create_pool(size_t sizeOfEachBlock, unsigned int numOfBlocks)
		{
			_blocks_num = numOfBlocks;
			_block_size = sizeOfEachBlock;
			_beginning_pool = new unsigned char[_block_size * _blocks_num];
			_free_blocks_num = numOfBlocks;
			_next_free_block = _beginning_pool;
		}

		void destroypool()
		{
			delete[] _beginning_pool;
			_beginning_pool = nullptr;
		}

		unsigned char* addr_from_index(unsigned int i) const
		{
			return _beginning_pool + (i * _block_size);
		}

		unsigned int index_from_addr(const unsigned char* p) const
		{
			return (((unsigned int)(p - _beginning_pool)) / _block_size);
		}

		void* allocate()
		{
			if (_initialized_num < _blocks_num)
			{
				unsigned int* p = (unsigned int*)addr_from_index(_initialized_num);
				*p = _initialized_num + 1;
				_initialized_num++;
			}
			void* ret = nullptr;
			if (_free_blocks_num > 0)
			{
				ret = (void*)_next_free_block;
				--_free_blocks_num;
				if (_free_blocks_num != 0)
				{
					_next_free_block = addr_from_index(*((unsigned int*)_next_free_block));
				}
				else
				{
					_next_free_block = nullptr;
				}
			}
			return ret;
		}

		void de_allocate(void* p)
		{
			if (_next_free_block != nullptr)
			{
				(*(unsigned int*)p) = index_from_addr(_next_free_block);
				_next_free_block = (unsigned char*)p;
			}
			else
			{
				*((unsigned int*)p) = _blocks_num;
				_next_free_block = (unsigned char*)p;
			}
			++_free_blocks_num;
		}
	};
}

#endif
