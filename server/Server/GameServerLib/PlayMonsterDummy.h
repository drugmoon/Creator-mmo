#pragma once
#include "playmonster.h"
#include "PlayNpc.h"
#include "PlaySlave.h"
#include "PlayNeutral.h"
#include "PlayMonsterBossNomove.h"
#include "PlayMonsterHelper.h"
#include "PlayMonsterNomove.h"
#include "PlayMonsterPuppet.h"
#include "PlayMonsterRemote.h"
#include "PlayMonsterRemoteNomove.h"
#include "PlayMonsterCaller.h"
#include "PlayNeutralRam.h"
#include "leudgrid/MemoryPool.h"
#include "PlayMonsterExplode.h"
namespace OGSLib
{
	class PlayMonsterDummy :
		public PlayMonster,
		public LEUD::PoolMemoryBase< PlayMonsterDummy >
	{
	public:
		PlayMonsterDummy(void);
		virtual ~PlayMonsterDummy(void);
	};
	class PlayNpcDummy :
		public PlayNpc,
		public LEUD::PoolMemoryBase< PlayNpcDummy >
	{
	public:
		PlayNpcDummy(GameServerLib* gsl);
		virtual ~PlayNpcDummy(void);
	};
	class PlaySlaveDummy :
		public PlaySlave,
		public LEUD::PoolMemoryBase< PlaySlaveDummy >
	{
	public:
		PlaySlaveDummy(void);
		virtual ~PlaySlaveDummy(void);
	};
	class PlayDart2Dummy :
		public PlayDart2,
		public LEUD::PoolMemoryBase< PlayDart2Dummy >
	{
	public:
		PlayDart2Dummy(void);
		virtual ~PlayDart2Dummy(void);
	};
	class PlaySlaveFashiDummy :
		public PlaySalveFashi,
		public LEUD::PoolMemoryBase< PlaySlaveFashiDummy >
	{
	public:
		PlaySlaveFashiDummy(void);
		virtual ~PlaySlaveFashiDummy(void);
	};

	class PlaySlaveStatueDummy:
		public PlaySlaveStatue,
		public LEUD::PoolMemoryBase<PlaySlaveStatueDummy>
	{
	public:
		PlaySlaveStatueDummy(void);
		virtual ~PlaySlaveStatueDummy(void);
	};

	class PlayNeutralDummy :
		public PlayNeutral,
		public LEUD::PoolMemoryBase< PlayNeutralDummy >
	{
	public:
		PlayNeutralDummy(void);
		virtual ~PlayNeutralDummy(void);
	};
	class PlayMonsterNomoveDummy :
		public PlayMonsterNomove,
		public LEUD::PoolMemoryBase< PlayMonsterNomoveDummy >
	{
	public:
		PlayMonsterNomoveDummy(void);
		virtual ~PlayMonsterNomoveDummy(void);
	};
	class PlayMonsterRemoteDummy :
		public PlayMonsterRemote,
		public LEUD::PoolMemoryBase< PlayMonsterRemoteDummy >
	{
	public:
		PlayMonsterRemoteDummy(void);
		virtual ~PlayMonsterRemoteDummy(void);
	};
	class PlayMonsterHelperDummy :
		public PlayMonsterHelper,
		public LEUD::PoolMemoryBase< PlayMonsterHelperDummy >
	{
	public:
		PlayMonsterHelperDummy(void);
		virtual ~PlayMonsterHelperDummy(void);
	};
	class PlayMonsterRemoteNomoveDummy :
		public PlayMonsterRemoteNomove,
		public LEUD::PoolMemoryBase< PlayMonsterRemoteNomoveDummy >
	{
	public:
		PlayMonsterRemoteNomoveDummy(void);
		virtual ~PlayMonsterRemoteNomoveDummy(void);
	};
	class PlayMonsterBossNomoveDummy :
		public PlayMonsterBossNomove,
		public LEUD::PoolMemoryBase< PlayMonsterBossNomoveDummy >
	{
	public:
		PlayMonsterBossNomoveDummy(void);
		virtual ~PlayMonsterBossNomoveDummy(void);
	};
	class PlayMonsterPuppetDummy :
		public PlayMonsterPuppet,
		public LEUD::PoolMemoryBase< PlayMonsterPuppetDummy >
	{
	public:
		PlayMonsterPuppetDummy(void);
		virtual ~PlayMonsterPuppetDummy(void);
	};
	class PlayNeutralRamDummy :
		public PlayNeutralRam,
		public LEUD::PoolMemoryBase< PlayNeutralRamDummy >
	{
	public:
		PlayNeutralRamDummy(void);
		virtual ~PlayNeutralRamDummy(void);
	};
	class PlayMonsterPlantDummy :
		public PlayMonsterPlant,
		public LEUD::PoolMemoryBase< PlayMonsterPlantDummy >
	{
	public:
		PlayMonsterPlantDummy(void);
		virtual ~PlayMonsterPlantDummy(void);
	};
	class PlayMonsterCollectionDummy:
		public PlayMonsterCollection,
		public LEUD::PoolMemoryBase<PlayMonsterCollectionDummy>
	{
	public:
		PlayMonsterCollectionDummy(void);
		virtual ~PlayMonsterCollectionDummy(void);
	};
	class PlayMonsterCallerDummy:
		public PlayMonsterCaller,
		public LEUD::PoolMemoryBase<PlayMonsterCallerDummy>
	{
	public:
		PlayMonsterCallerDummy(void);
		virtual ~PlayMonsterCallerDummy(void);
	};
	class PlayMonsterExplodeDummy:
		public PlayMonsterExplode,
		public LEUD::PoolMemoryBase<PlayMonsterExplodeDummy>
	{
	public:
		PlayMonsterExplodeDummy(void);
		virtual ~PlayMonsterExplodeDummy(void);
	};
}
