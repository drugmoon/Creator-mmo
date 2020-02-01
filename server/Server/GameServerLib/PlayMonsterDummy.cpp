#include "StdAfx.h"
#include "PlayMonsterDummy.h"
namespace OGSLib
{
	PlayMonsterDummy::PlayMonsterDummy(void)
	{
	}
	PlayMonsterDummy::~PlayMonsterDummy(void)
	{
	}
	PlayNpcDummy::PlayNpcDummy(GameServerLib* gsl):PlayNpc(gsl)
	{
	}
	PlayNpcDummy::~PlayNpcDummy(void)
	{
	}
	PlaySlaveDummy::PlaySlaveDummy(void)
	{
	}
	PlaySlaveDummy::~PlaySlaveDummy(void)
	{
	}
	PlayDart2Dummy::PlayDart2Dummy(void)
	{
	}
	PlayDart2Dummy::~PlayDart2Dummy(void)
	{
	}
	PlayNeutralDummy::PlayNeutralDummy(void)
	{
	}
	PlayNeutralDummy::~PlayNeutralDummy(void)
	{
	}
	PlayMonsterNomoveDummy::PlayMonsterNomoveDummy(void)
	{
	}
	PlayMonsterNomoveDummy::~PlayMonsterNomoveDummy(void)
	{
	}
	PlayMonsterRemoteDummy::PlayMonsterRemoteDummy(void)
	{
	}
	PlayMonsterRemoteDummy::~PlayMonsterRemoteDummy(void)
	{
	}
	PlayMonsterHelperDummy::PlayMonsterHelperDummy(void)
	{
	}
	PlayMonsterHelperDummy::~PlayMonsterHelperDummy(void)
	{
	}
	PlayMonsterRemoteNomoveDummy::PlayMonsterRemoteNomoveDummy(void)
	{
	}
	PlayMonsterRemoteNomoveDummy::~PlayMonsterRemoteNomoveDummy(void)
	{
	}
	PlayMonsterBossNomoveDummy::PlayMonsterBossNomoveDummy(void)
	{
	}
	PlayMonsterBossNomoveDummy::~PlayMonsterBossNomoveDummy(void)
	{
	}
	PlayMonsterPuppetDummy::PlayMonsterPuppetDummy(void)
	{
	}
	PlayMonsterPuppetDummy::~PlayMonsterPuppetDummy(void)
	{
	}
	PlayNeutralRamDummy::PlayNeutralRamDummy(void)
	{
	}
	PlayNeutralRamDummy::~PlayNeutralRamDummy(void)
	{
	}
	PlayMonsterPlantDummy::PlayMonsterPlantDummy(void)
	{
	}
	PlayMonsterPlantDummy::~PlayMonsterPlantDummy(void)
	{
	}
	PlayMonsterCollectionDummy::PlayMonsterCollectionDummy(void)
	{
	}
	PlayMonsterCollectionDummy::~PlayMonsterCollectionDummy(void)
	{
	}

	PlayMonsterCallerDummy::PlayMonsterCallerDummy(void)
	{
	}
	PlayMonsterCallerDummy::~PlayMonsterCallerDummy(void)
	{
	}
	PlayMonsterExplodeDummy::PlayMonsterExplodeDummy(void)
	{
	}
	PlayMonsterExplodeDummy::~PlayMonsterExplodeDummy(void)
	{
	}

	PlaySlaveFashiDummy::PlaySlaveFashiDummy(void)
	{

	}

	PlaySlaveFashiDummy::~PlaySlaveFashiDummy(void)
	{

	}

	PlaySlaveStatueDummy::PlaySlaveStatueDummy(void)
	{

	}

	PlaySlaveStatueDummy::~PlaySlaveStatueDummy(void)
	{

	}

}

LEUD::PoolMemory<OGSLib::PlayMonsterDummy>				LEUD::PoolMemoryBase<OGSLib::PlayMonsterDummy>::memPool("PlayMonsterDummy");
LEUD::PoolMemory<OGSLib::PlayNpcDummy>					LEUD::PoolMemoryBase<OGSLib::PlayNpcDummy>::memPool("PlayNpcDummy");
LEUD::PoolMemory<OGSLib::PlaySlaveDummy>				LEUD::PoolMemoryBase<OGSLib::PlaySlaveDummy>::memPool("PlaySlaveDummy");
LEUD::PoolMemory<OGSLib::PlaySlaveFashiDummy>			LEUD::PoolMemoryBase<OGSLib::PlaySlaveFashiDummy>::memPool("PlaySlaveFashiDummy");
LEUD::PoolMemory<OGSLib::PlaySlaveStatueDummy>			LEUD::PoolMemoryBase<OGSLib::PlaySlaveStatueDummy>::memPool("PlaySlaveStatueDummy");
LEUD::PoolMemory<OGSLib::PlayDart2Dummy>				LEUD::PoolMemoryBase<OGSLib::PlayDart2Dummy>::memPool("PlayDartDummy");
LEUD::PoolMemory<OGSLib::PlayNeutralDummy>				LEUD::PoolMemoryBase<OGSLib::PlayNeutralDummy>::memPool("PlayNeutralDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterNomoveDummy>		LEUD::PoolMemoryBase<OGSLib::PlayMonsterNomoveDummy>::memPool("PlayMonsterNomoveDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterRemoteDummy>		LEUD::PoolMemoryBase<OGSLib::PlayMonsterRemoteDummy>::memPool("PlayMonsterRemoteDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterHelperDummy>		LEUD::PoolMemoryBase<OGSLib::PlayMonsterHelperDummy>::memPool("PlayMonsterHelperDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterRemoteNomoveDummy>	LEUD::PoolMemoryBase<OGSLib::PlayMonsterRemoteNomoveDummy>::memPool("PlayMonsterRemoteNomoveDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterBossNomoveDummy>	LEUD::PoolMemoryBase<OGSLib::PlayMonsterBossNomoveDummy>::memPool("PlayMonsterBossNomoveDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterPuppetDummy>		LEUD::PoolMemoryBase<OGSLib::PlayMonsterPuppetDummy>::memPool("PlayMonsterPuppetDummy");
LEUD::PoolMemory<OGSLib::PlayNeutralRamDummy>			LEUD::PoolMemoryBase<OGSLib::PlayNeutralRamDummy>::memPool("PlayNeutralRamDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterPlantDummy>			LEUD::PoolMemoryBase<OGSLib::PlayMonsterPlantDummy>::memPool("PlayMonsterPlantDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterCollectionDummy>	LEUD::PoolMemoryBase<OGSLib::PlayMonsterCollectionDummy>::memPool("PlayMonsterCollectionDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterCallerDummy>		LEUD::PoolMemoryBase<OGSLib::PlayMonsterCallerDummy>::memPool("PlayMonsterCallerDummy");
LEUD::PoolMemory<OGSLib::PlayMonsterExplodeDummy>		LEUD::PoolMemoryBase<OGSLib::PlayMonsterExplodeDummy>::memPool("PlayMonsterExplodeDummy");