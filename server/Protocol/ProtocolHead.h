#pragma once

namespace ProtocolHead
{
	size_t HeadLen();
	void MakeHead(char* buf,unsigned short len);
	unsigned short FindHead(const char* buf);
	//void MakeHead(Stream_t* stream,unsigned short len);
	//unsigned short FindHead(Stream_t* stream);
}