#pragma once
#include "memoryPool.h"
namespace LEUD
{
	template<int T>
	class MemoryBlock : public PoolMemoryBase< MemoryBlock< T > >{
	private:
		MemoryBlock& operator=(const MemoryBlock&);
	public:
		size_t m_Used;
		MemoryBlock<T>* m_Next;
		char m_Data[T];
	public:
		MemoryBlock(const MemoryBlock& o){
			m_Used = o.m_Used;
			m_Next = 0;
			memcpy(m_Data,o.m_Data,T);
		}
		MemoryBlock():m_Used(0),m_Next(0){
		}
		char* data( ){ 
			return m_Data;
		}
		static size_t	capacity( ){ 
			return T;
		}
		size_t size() { 
			return m_Used; 
		}
		size_t size(size_t Used) { 
			return m_Used = Used; 
		}
	};
}
typedef LEUD::MemoryBlock<248> Block_t;
typedef LEUD::MemoryBlock<504> Block512_t;
typedef LEUD::MemoryBlock<1016> Block1024_t;

