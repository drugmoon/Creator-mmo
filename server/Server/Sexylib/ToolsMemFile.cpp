#include "StdAfx.h"
#include "leudgrid/ToolsMemFile.h"
namespace LEUD
{
	ToolsMemFile::ToolsMemFile(const char* data,size_t len)
	{
		mData = data;
		mLen = len;
		mPos = 0;
	}
	ToolsMemFile::~ToolsMemFile(void)
	{
	}
	size_t ToolsMemFile::read_line(char* buf,size_t len)
	{
		size_t readed=0;
		size_t op = mPos;
		while( readed < len && mPos < mLen )
		{
			if( mData[mPos] == 13 )
			{
				mPos++;
				if( mPos<mLen )
				{
					if( mData[mPos] == 10 )
					{
						mPos ++;
					}
				}
				break;
			}
			buf[readed++] = mData[mPos++];
		}
		if( readed < len )
		{
			buf[readed] = 0;
		}
		else
		{
			buf[len-1] = 0;
		}
		return mPos-op;
	}
}