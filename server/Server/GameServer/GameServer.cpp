// GameServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"winmm.lib")

#include "leudgrid/ServerMain.h"
#include "GameServerApp.h"

int main(int argc, char** argv)
{
	//_CrtSetBreakAlloc(6144);
	std::locale::global(std::locale(std::locale::classic(),"",std::locale::ctype|std::locale::time)); 
	printf( "Game Server \n" );
	printf( "Build Time : %s %s \n", __DATE__,__TIME__ );
	printf( "Enter help to get more infomation! \n" );
	printf( "!!!!Enter exit to close server!!!! \n" );
	printf( "!!Do not close console directly!!! \n\n" );

	CGameServerApp theApp;
	return LEUD::TheMain( argc, argv );
}

