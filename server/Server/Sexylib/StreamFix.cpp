#include "stdafx.h"
#include "leudgrid/StreamFix.h"

namespace LEUD
{
	StreamFix& StreamFix::In( const char* buffer, size_t size )
	{
		if(*this)
		{
			if( m_Buffer && m_WritePos + size <= m_Size )
			{
				memcpy(m_Buffer+m_WritePos,buffer,size);
				m_WritePos += size;
			}else
			{
				setState(failbit,goodbit);
			}
		}
		return *this;
	}
	StreamFix& StreamFix::Out( char* buffer,size_t size )
	{
		if(*this)
		{
			const char* buf = 0;
			if( m_Buffer ) buf = m_Buffer;
			if( m_ConstBuffer ) buf = m_ConstBuffer;
			if( buf && m_ReadPos + size <= m_WritePos )
			{
				memcpy(buffer,buf+m_ReadPos,size);
				m_ReadPos += size;
			}
			else
			{
				setState(failbit,goodbit);
			}			
		}
		return *this;
	}
	size_t StreamFix::findchar( char end )
	{
		if(*this)
		{
			const char* buf = 0;
			if( m_Buffer ) buf = m_Buffer;
			if( m_ConstBuffer ) buf = m_ConstBuffer;
			size_t length = 0;
			for( size_t i = m_ReadPos;i<m_WritePos;i++ )
			{
				length ++;
				if( buf[i] == end )
				{
					return length;
				}				
			}
		}
		return 0;
	}
}