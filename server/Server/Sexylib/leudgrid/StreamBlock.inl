#pragma once
#include "StreamBlock.h"
namespace LEUD
{
	template<class BLOCK>
		inline	StreamBlock< BLOCK >::StreamBlock():m_Size(0),m_ReadPos(0),m_First(0),m_Last(0),m_Prev(0),m_Next(0)
	{
		m_First=&m_Data;
		m_Last =&m_Data;
		m_Last->m_Next=0;
	}

	template<class BLOCK>
		inline StreamBlock< BLOCK >& StreamBlock< BLOCK >::In( const char* buffer, size_t size )
	{
		BLOCK* block = NULL;
		if( ! m_First ){
			push_back(new BLOCK);
		}
		size_t copyed = 0;
		for(;;)
		{
			block = m_Last;
			size_t free = block->capacity() - block->size();
			size_t left = size - copyed;
			if( 0 == free )
			{
				push_back(new BLOCK);
				continue;
			}
			if( free < left )//bu gou
			{
				memcpy( block->data() + block->size(),buffer + copyed,free );
				copyed += free;
				block->size( block->capacity() );
				push_back(new BLOCK);
			}
			else//gou
			{
				memcpy( block->data() + block->size(),buffer + copyed,left);
				copyed = size;
				block->size(block->size()+left);
			}
			if( copyed >= size )
			{
				break;
			}
		}
		m_Size += copyed;
		return *this;
	}
	template<class BLOCK>
		inline StreamBlock< BLOCK >& StreamBlock< BLOCK >::Out( char* buffer,size_t size )
	{
		if( *this ){
			size_t pt = m_ReadPos;
			BLOCK* block;
			size_t readed = 0;
			for( block = m_First;
				block;
				block = block->m_Next )
			{
				if( block->size() <= pt ) //readed
				{
					pt -= block->size();
				}
				else // can read
				{
					for( ;
						block;
						block = block->m_Next )
					{
						size_t unread = block->size()-pt;
						size_t left = size - readed;
						if( unread < left )//bu gou
						{
							memcpy(buffer+readed,block->data()+pt,unread);
							pt = 0;
							m_ReadPos += unread;
							readed += unread;
						}
						else // gou
						{
							memcpy(buffer+readed,block->data()+pt,left);
							pt = 0;
							m_ReadPos += left;
							readed = size;
						}
						if( readed == size )
						{
							break;
						}
					}
					break;
				}
			}
			if( readed != size )
			{
				setState(failbit,goodbit);
			}
		}
		return *this;
	}
	template<class BLOCK>
		inline	size_t StreamBlock< BLOCK >::clear(size_t num)
	{
		m_Size -= num;
		for( ;m_First; )
		{
			BLOCK* block = m_First;
			if( num > 0 )
			{
				if( block->size() <= num )
				{
					num -= block->size();
					m_First = m_First->m_Next;
					if( ! m_First ){
						m_Last = 0;
					}
					if( block!=&m_Data )
					{
						delete block;
					}
				}else{
					memcpy(block->data(),&block->data()[num],block->size()-num);
					block->size(block->size()-num);
					num = 0;
				}
			}else{
				break;
			}
		}
		seek(0);
		m_Size += num;
		clearbit();
		return num;
	}
	template<class BLOCK>
		inline size_t StreamBlock< BLOCK >::findchar( char end )
	{
		size_t size = 0;
		size_t pass = 0;
		BLOCK* block = m_First;
		for( ;
			block;
			block = block->m_Next )
		{
			if( pass + block->size() < m_ReadPos )
			{
				pass += block->size();
				continue;
			}
			size_t i;
			for( i = 0;i < block->size();i ++ )
			{
				if( pass + i < m_ReadPos )
				{
					continue;
				}
				size++;
				if( end == block->data()[i] ){
					break;
				}
			}
			if( i < block->size() )
			{
				break;
			}
			pass += block->size();
		}
		if( ! block )
		{
			return 0;
		}
		return size;
	}
}