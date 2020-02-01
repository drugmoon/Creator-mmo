#include "stdafx.h"
#include "leudgrid/MemoryBlock.h"

LEUD::PoolMemory<Block_t>							LEUD::PoolMemoryBase<Block_t>::memPool("Block_t");
LEUD::PoolMemory<Block512_t>						LEUD::PoolMemoryBase<Block512_t>::memPool("Block512_t");
LEUD::PoolMemory<Block1024_t>						LEUD::PoolMemoryBase<Block1024_t>::memPool("Block1024_t");
