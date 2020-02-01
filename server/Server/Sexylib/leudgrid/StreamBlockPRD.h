#pragma once 
#include "leudgrid/MemoryBlockPRD.h"

namespace LEUD
{
	template<class BLOCK>
	class StreamBlock;
}
typedef LEUD::StreamBlock<Block_t> Stream_t;
typedef LEUD::StreamBlock<Block512_t> Stream512_t;
typedef LEUD::StreamBlock<Block1024_t> Stream1024_t;
