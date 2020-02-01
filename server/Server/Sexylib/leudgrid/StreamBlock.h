#pragma once
#include "StreamBase.h"
#include "MemoryBlock.h"
namespace LEUD
{
	template<class BLOCK>
	class StreamBlock : public PoolMemoryBase< StreamBlock< BLOCK > >, public StreamBase< StreamBlock< BLOCK > >{
	public:
		class iterator{
			friend StreamBlock;
			BLOCK* m_Block;
		public:
			iterator( BLOCK* bl ) : m_Block(bl){
			}
			BLOCK& operator*() const{
				return *m_Block;
			}
			BLOCK* operator->() const{
				return m_Block;
			}
			iterator& operator++(){
				m_Block = m_Block->m_Next;
				return (*this);
			}
			bool	Is_Done(void){
				return m_Block == 0;
			}
		};
		iterator begin( void )	{
			return iterator( m_First );
		}
	private:
		StreamBlock& operator=(const StreamBlock&);
		StreamBlock(const StreamBlock&);
	public:
		size_t		m_Size;
		size_t		m_ReadPos;
		BLOCK		m_Data;
		BLOCK*		m_First;
		BLOCK*		m_Last;
		void*		m_Prev;
		void*		m_Next;
	public:
		StreamBlock();
		~StreamBlock(){
			while(m_First){
				m_Last = m_First;
				m_First = m_First->m_Next;
				if( m_Last != &m_Data )
				{
					delete m_Last;
				}
			}
		}
	public:
		StreamBlock& merge(StreamBlock& s){
			for( Stream_t::iterator pos = s.begin();
				! pos.Is_Done();
				 )
			{
				Block_t* b = &(*pos);
				++pos;
				push_back(b);
			}
			s.m_First = 0;
			s.m_Last = 0;
			s.m_Size = 0;
			return *this;
		}
		StreamBlock& push_back(BLOCK* block){
			if( m_Last ){
				m_Last->m_Next = block;
				m_Last = block;
			}else{
				m_First = block;
				m_Last = block;
			}
			m_Last->m_Next = 0;
			m_Size += block->size();
			return *this;
		}
		StreamBlock& push_front(BLOCK* block){
			if( m_First ){
				block->m_Next = m_First;
				m_First = block;
			}else{
				m_First = block;
				m_Last = block;
				m_Last->m_Next = 0;
			}
			m_Size += block->size();
			return *this;
		}
		StreamBlock& In( const char* buffer, size_t size );
		StreamBlock& operator<<(BLOCK* block){
			return push_back( block );
		}
		using StreamBase<StreamBlock>::operator <<;
	public:
		size_t size(){
			return m_Size;
		}
		void size(size_t s){
			m_Size = s;
			m_First->size(s);
		}
		size_t seek(){
			return m_ReadPos;
		}
		void seek(size_t seek){
			m_ReadPos = seek;
		}
		size_t left(){
			return m_Size-m_ReadPos;
		}
		size_t clear(size_t num);
		size_t findchar( char end );
		StreamBlock& Out( char* buffer,size_t size );
		using StreamBase<StreamBlock>::operator >>;
	};
}
typedef LEUD::StreamBlock<Block_t> Stream_t;
typedef LEUD::StreamBlock<Block512_t> Stream512_t;
typedef LEUD::StreamBlock<Block1024_t> Stream1024_t;
