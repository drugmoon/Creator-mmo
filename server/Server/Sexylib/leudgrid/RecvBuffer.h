#pragma once

namespace LEUD
{
	class RecvBuffer
	{
		static const int mFixBufferSize=1024;
		char mFixBuffer[mFixBufferSize];
		char* mDynamicBuffer;
		size_t mBufferSize;
		size_t mUsedSize;
		size_t mFreedSize;
	public:
		RecvBuffer();
		~RecvBuffer();

		char* free_data();
		size_t free_size();
		char* used_data(size_t pos);
		size_t used_size();

		bool use(size_t len);

		bool free(size_t len);
	};
}