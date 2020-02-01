#include "stdafx.h"
#include "leudgrid/StreamBlock.h"

LEUD::PoolMemory<Stream_t>							LEUD::PoolMemoryBase<Stream_t>::memPool("Stream_t");
LEUD::PoolMemory<Stream512_t>						LEUD::PoolMemoryBase<Stream512_t>::memPool("Stream512_t");
LEUD::PoolMemory<Stream1024_t>						LEUD::PoolMemoryBase<Stream1024_t>::memPool("Stream1024_t");
