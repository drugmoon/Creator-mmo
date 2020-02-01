#include "stdafx.h"
#include "leudgrid/recvbuffer.h"

namespace LEUD
{
	RecvBuffer::RecvBuffer()
	{
		mDynamicBuffer = 0;
		mBufferSize = mFixBufferSize;
		mUsedSize = 0;
		mFreedSize = 0;
	}
	RecvBuffer::~RecvBuffer()
	{
		if( mDynamicBuffer )
		{
			delete[] mDynamicBuffer;
			mDynamicBuffer=0;
		}
	}
	char* RecvBuffer::free_data()
	{
		char* buf = 0;
		if( mDynamicBuffer ) buf = mDynamicBuffer; else buf = mFixBuffer;
		if( mFreedSize > 0 )
		{
			memcpy(buf,&buf[mFreedSize],mUsedSize-mFreedSize);
			mUsedSize -= mFreedSize;
			mFreedSize = 0;			
		}
		if( mUsedSize+100 >= mBufferSize )
		{
			char* newbuf = new char[mBufferSize*2];
			memcpy(newbuf,buf,mUsedSize);
			if( mDynamicBuffer )
			{
				delete[] mDynamicBuffer;
			}
			mDynamicBuffer = newbuf;
			buf = mDynamicBuffer;
			mBufferSize *= 2;
		}
		return &buf[mUsedSize];
	}
	size_t RecvBuffer::free_size()
	{
		if( mBufferSize-mUsedSize < 100 )
		{
			free_data();
		}
		return mBufferSize-mUsedSize;
	}
	char* RecvBuffer::used_data(size_t pos)
	{
		char* ret=0;
		if( mFreedSize+pos < mUsedSize )
		{
			if( mDynamicBuffer )
			{
				ret = &mDynamicBuffer[mFreedSize+pos];
			}
			else
			{
				ret = &mFixBuffer[mFreedSize+pos];
			}
		}
		return ret;
	}
	size_t RecvBuffer::used_size()
	{
		return mUsedSize-mFreedSize;
	}
	bool RecvBuffer::use(size_t len)
	{
		mUsedSize+=len;
		return 0;
	}
	bool RecvBuffer::free(size_t len)
	{
		mFreedSize+=len;
		return 0;
	}
}