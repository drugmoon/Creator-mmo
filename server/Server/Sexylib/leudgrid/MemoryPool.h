#pragma once
#include "leudgrid/toolsthread.h"
#include <assert.h>
namespace LEUD
{
	void AddMemoryPoolItem(const char* name,size_t* max,size_t* free,size_t* used,size_t size);
	size_t PoolMemoryGetMemorySize();
	void PoolMemoryDumpInfo();
	template<class T>
	class PoolMemory{
	public:
		PoolMemory(const char* name){
			next=0;m_max =0;m_free=0;m_used=0;
			m_name=name;
			m_max_slot = -1;
			AddMemoryPoolItem(name,&m_max,&m_free,&m_used,sizeof(T));
		}
		~PoolMemory(){
			CriticalSection::Lock lock(critical_section);
			//dump_info();
			assert(m_max == m_free+m_used);
			void* NextPtr = next;
			for( NextPtr = next; NextPtr != 0; NextPtr = next )
			{
				next = *(void**)next;
				delete[] NextPtr;
			}
		}
		void dump_info(){
			if( m_name )
			{
				printf( "FreeList:%p sizeof<%5d> max<%5d> free<%5d> used<%5d> name:%s\n",this,sizeof(T),m_max,m_free,m_used,m_name );
			}
			else
			{
				printf( "FreeList:%p sizeof<%5d> max<%5d> free<%5d> used<%5d>",this,sizeof(T),m_max,m_free,m_used );
			}
		}
		void * alloc( size_t size ){
			if( sizeof( T ) != size )
				return ::operator new( size );
			CriticalSection::Lock lock(critical_section);
			if( ! next )
			{
				expandTheFreeList();
			}
			void* head = next;
			next = *(void**)next;
			m_free--;m_used++;
			return head;
		}
		void free( void* se, size_t size ){
			if( 0 == size ) return;
			if( sizeof( T ) != size ){
				::operator delete( se );
				return;
			}
			{
				CriticalSection::Lock lock(critical_section);
				*(void**)se = next;
				next = se;
				m_free++;m_used--;
			}
		}
		void release( size_t num ){
			CriticalSection::Lock lock(critical_section);
			void* NextPtr = next;
			for( NextPtr = next,size_t i = 0; i<num && NextPtr != 0; NextPtr = next,i++ )
			{
				next = *(void**)next;
				delete[] NextPtr;
			}
		}
	public:
		size_t m_max,m_free,m_used;
		const char* m_name;
		size_t m_max_slot;
	private:
		void* next;
		CriticalSection critical_section;
		enum{EXPANSION_SIZE = 32 };
		void expandTheFreeList( int howMany = EXPANSION_SIZE ){
			if( m_max_slot > 0 && m_max > m_max_slot ) return;
			CriticalSection::Lock lock(critical_section);
			size_t size = ( sizeof(T)>sizeof(PoolMemory<T>*) ? sizeof(T) : sizeof(PoolMemory<T>*) );
			void* runner = new char[size];
			next = runner;
			m_free++;m_max++;
			for( int i = 0; i < howMany; i ++ )
			{
				runner = *(void**)runner = new char[size];
				m_free++;m_max++;
			}
			if( m_max > 100000 )
			{
				//dump_info();
			}
			*(void**)runner = 0;
		}
	};
	template< class T >
	class PoolMemoryBase{
	public:
		void* operator new(size_t nSize, size_t n, const char * lpszFileName, int nLine){
			n;
#if defined(_DEBUG) && defined(_DEBUG_FREELIST_LEAK)
			return ::operator new(nSize, 1, lpszFileName, nLine);
#else
			return memPool.alloc( nSize );
#endif
		}
		void * operator new( size_t nSize ){
#if defined(_DEBUG) && defined(_DEBUG_FREELIST_LEAK)
			return ::operator new(nSize, 1, lpszFileName, nLine);
#else
			return memPool.alloc( nSize );
#endif
		}
		void operator delete( void * se,size_t size,const char * lpszFileName, int nLine ){
#ifdef _DEBUG
			PoolMemoryBase<T>* temp = static_cast<T*>( 0 );temp;
#endif
#if defined(_DEBUG) && defined(_DEBUG_FREELIST_LEAK)
			::operator delete(se);
#else
			memPool.free( se, size );
#endif
		}
		void operator delete( void * se,size_t size ){
#ifdef _DEBUG
			PoolMemoryBase<T>* temp = static_cast<T*>( 0 );temp;
#endif
#if defined(_DEBUG) && defined(_DEBUG_FREELIST_LEAK)
			::operator delete(se);
#else
			memPool.free( se, size );
#endif
		}
	public:
		static PoolMemory< T > memPool;
	};
}