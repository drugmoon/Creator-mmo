#pragma once
namespace LEUD
{
	class ToolsMemFile
	{
		const char* mData;
		size_t mLen;
		size_t mPos;
	public:
		ToolsMemFile(const char* data,size_t len);
		virtual ~ToolsMemFile(void);

		size_t read_line(char* buf,size_t len);
	};
}