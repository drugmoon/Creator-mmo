#include "stdafx.h"
namespace ProtocolHead
{
	size_t HeadLen()
	{
		return sizeof(unsigned short) * 2;
	}
	void MakeHead(char* buf,unsigned short len)
	{
		memcpy(buf,(char*)&len,sizeof(len));
		unsigned short revlen = ~len;
		memcpy(&buf[2],(char*)&revlen,sizeof(revlen));
		int* t=(int*)buf;
		*t = *t ^ 0xA6E839CD;
		char c0 = buf[0];
		char c1 = buf[1];
		char c2 = buf[2];
		char c3 = buf[3];
		buf[0] = c2;
		buf[1] = c0;
		buf[2] = c3;
		buf[3] = c1;
	}
	unsigned short FindHead(const char* buf)
	{
		char bb[4];
		memcpy(bb,buf,4);
		char c0 = bb[0];
		char c1 = bb[1];
		char c2 = bb[2];
		char c3 = bb[3];
		bb[2] = c0;
		bb[0] = c1;
		bb[3] = c2;
		bb[1] = c3;
		int* t=(int*)bb;
		*t = *t ^ 0xA6E839CD;
		unsigned short len = *(unsigned short*)bb;
		unsigned short revlen = *(unsigned short*)&bb[sizeof(unsigned short)];
		if( (unsigned short)~len != revlen )
			return 0;
		return len;
	}
	//void MakeHead(Stream_t* stream,unsigned short len)
	//{
	//	unsigned short revlen=~len;
	//	(*stream)<<len<<revlen;
	//}
	//unsigned short FindHead(Stream_t* stream)
	//{
	//	unsigned short len = 1;
	//	unsigned short revlen = 2;
	//	(*stream)>>len>>revlen;
	//	if( (*stream) && (unsigned short)~len != revlen )
	//		return 0;
	//	return len;
	//}
}