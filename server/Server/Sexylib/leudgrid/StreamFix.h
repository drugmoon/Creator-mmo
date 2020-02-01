#pragma once
#include "leudgrid/StreamBase.h"
namespace LEUD
{
	class StreamFix : public StreamBase< StreamFix >
	{
	private:
		StreamFix& operator=(const StreamFix&);
		StreamFix(const StreamFix&);
	public:
		const char* m_ConstBuffer;
		char*		m_Buffer;
		size_t		m_Size;
		size_t		m_ReadPos;
		size_t		m_WritePos;
	public:
		StreamFix(const char* buffer,size_t size,size_t datasize=0):m_ConstBuffer(buffer),m_Buffer(0),m_Size(size),m_ReadPos(0),m_WritePos(datasize){
		}
		StreamFix(char* buffer,size_t size,size_t datasize=0):m_ConstBuffer(0),m_Buffer(buffer),m_Size(size),m_ReadPos(0),m_WritePos(datasize){
		}
		~StreamFix(){
		}
	public:
		StreamFix& In( const char* buffer, size_t size );
		using StreamBase<StreamFix>::operator <<;
	public:
		size_t size(){
			return m_WritePos;
		}
		size_t seek(){
			return m_ReadPos;
		}
		void seek(size_t seek){
			m_ReadPos = seek;
		}
		size_t left(){
			return m_WritePos-m_ReadPos;
		}
		size_t findchar( char end );
		StreamFix& Out( char* buffer,size_t size );
		using StreamBase<StreamFix>::operator >>;
	};
}