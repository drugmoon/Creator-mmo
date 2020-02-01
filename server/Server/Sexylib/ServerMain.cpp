#include "stdafx.h"
#include <iostream>
#include "leudgrid/DebugLeak.h"
#include "leudgrid/EventQueue.h"
#include "leudgrid/MonitorIOCP.h"
#include "leudgrid/ServerMain.h"
#include "leudgrid/Application.h"
#include "leudgrid/ServiceNT.h"
#include "leudgrid/ToolsPath.h"
#include <string>

namespace LEUD
{
	static SERVICE_STATUS_HANDLE	g_ServiceStatusHandle=NULL; 
	static Application*				g_Application=NULL;
	void SetApplication( Application* application ){
		g_Application = application;
	}

	static void ServiceStatus( SERVICE_STATUS_HANDLE handle, DWORD state ){
		SERVICE_STATUS status;
		status.dwServiceType				= SERVICE_WIN32_OWN_PROCESS;
		status.dwControlsAccepted			= SERVICE_ACCEPT_STOP;
		status.dwWin32ExitCode				= 0;
		status.dwServiceSpecificExitCode	= 0;
		status.dwCheckPoint					= 0;
		status.dwWaitHint					= 0;
		status.dwCurrentState				= state;        
		SetServiceStatus( handle,&status );
	}
	static void _stdcall ServiceCtrlHandler (DWORD Opcode){
		switch(Opcode)
		{
		case SERVICE_CONTROL_STOP:
			if( g_Application ){
				g_Application->Stop( );
				while( 0 != g_Application->QueryQuitState() ){
					ServiceStatus(g_ServiceStatusHandle,SERVICE_STOP_PENDING);
					Sleep(100);
				}
			}
			ServiceStatus(g_ServiceStatusHandle,SERVICE_STOPPED);
			break;
		}
		return;
	}
	static void _stdcall ServiceMain (DWORD argc, LPTSTR *argv){ 
		g_ServiceStatusHandle = RegisterServiceCtrlHandler( argv[0], ServiceCtrlHandler); 
		if (g_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		{
			ServiceStatus (g_ServiceStatusHandle,SERVICE_STOPPED);
		}else{
			if( g_Application ){
				g_Application->Start( argc,argv );
			}
			ServiceStatus (g_ServiceStatusHandle,SERVICE_RUNNING);
		}
		return; 
	}

	static int CommandHelp( int argc,char** argv ){
		if( 2 == argc )
		{
			std::cout << std::endl << argv[0] << " start_type [options]" << std::endl
				<< "/install    : Install service" << std::endl
				<< "   -service_name  : service name, defalut leudservice" << std::endl
				<< "   -service_disc  : service discription, default leudservice" << std::endl
				<< "   -command       : command, default this bin" << std::endl
				<< "/uninstall  : Uninstall service" << std::endl
				<< "   -service_name  : service name, defalut leudservice" << std::endl << std::endl
				<< "/console    : Start on console" << std::endl
				<< "/help       : Get help" << std::endl
				;
		}
		if( g_Application )
		{
			std::cout << g_Application->Help( argc-2, argv+2 ) << std::endl;
		}
		return 0;
	}
	static int CommandInstall( int argc, char** argv )
	{
		char exepath[1024];GetModuleFileName(NULL,exepath,1024);
		strncat_s( exepath,_countof(exepath), " /service",1023 );
		exepath[1023]=0;
		const char* service_name="leudservice";
		const char* service_disc="leudservice";
		const char* command=exepath;
		for(int i=2;i<argc;++i ){
			if( strcmp("-service_name",argv[i])==0 ){
				if( ++i < argc ){
					service_name = argv[i];
					strncat_s( exepath,_countof(exepath)," -service_name ",1023 );
					strncat_s( exepath,_countof(exepath),argv[i],1023 );
					exepath[1023]=0;
				}
			}
			else if( strcmp("-service_disc",argv[i])==0 ){
				if( ++i < argc ){
					service_disc = argv[i];
				}
			}
			else if( strcmp("-command",argv[i])==0 ){
				if( ++i < argc ){
					command = argv[i];
				}
			}
			else{
				strncat_s( exepath,_countof(exepath), " ",1023 );
				strncat_s( exepath,_countof(exepath), argv[i],1023 );
				exepath[1023]=0;
			}
		}
		bool result = LEUD::InstallService( service_name,service_disc,command );
		if( result ){
			printf( "service %s installed.\n",service_name );
		}else{
			printf( "install failed.\n" );
		}
		return result;
	}
	static int CommandUninstall( int argc, char** argv )
	{
		const char* service_name="leudservice";
		for(int i=2;i<argc;++i){
			if( strcmp("-service_name",argv[i])==0 ){
				if( ++i < argc ){
					service_name = argv[i];
				}
			}
		}
		bool result = LEUD::UninstallService( service_name );
		if( result ){
			printf( "service %s uninstalled.\n",service_name );
		}else{
			printf( "uninstall failed.\n" );
		}
		return result;
	}
	static int CommandConsole( int argc,char** argv )
	{
		if( g_Application )
		{
			const char* const PROMPT = "%>";
			const char* const PROMPT2= ">>";
			std::string line;
			char buf[512]={0};
			std::string source;
			std::string result;
			g_Application->Start(argc,argv);
			std::cout<<"start";
			while( ! g_Application->QueryStartState() )
			{
				std::cout<<".";
				Sleep(1000);
			}
			std::cout<<std::endl<<std::endl;
			std::cout<<PROMPT;
			while(std::cin.getline(buf,511))
			{
				line=buf;
				source += line + '\n';
				LEUD::Application::ExeCommandReturn ecret = g_Application->ExeCommand( line,result );
				printf( "%s\n",result.c_str() );
				result.clear();
				switch( ecret )
				{
				case LEUD::Application::EXECOMMAND_RETURN_NEXT:
					source.clear();
					std::cout<<PROMPT;
					break;
				case LEUD::Application::EXECOMMAND_RETURN_MORE:
					std::cout<<PROMPT2;
					break;
				case LEUD::Application::EXECOMMAND_RETURN_FINISH:
					g_Application->Stop();
					while( 0 != g_Application->QueryQuitState() ){
						Sleep(100);
					}
					break;
				case LEUD::Application::EXECOMMAND_RETURN_ERROR:
				default:
					source.clear();
				}
				if( ecret == LEUD::Application::EXECOMMAND_RETURN_FINISH )
				{
					break;
				}
			}
		}
		return 0;
	}
	static int CommandService( int argc,char** argv )
	{
		BinaryPathAsCurrentPath();
		char* service_name="leudservice";
		for(int i=2;i<argc;++i){
			if( std::string("-service_name") == argv[i] ){
				if( ++i < argc ){
					service_name = argv[i];
				}
			}
		}
		SERVICE_TABLE_ENTRY   DispatchTable[] = 
		{
			{ service_name,	ServiceMain}, 
			{ NULL,			NULL       },
		};
		StartServiceCtrlDispatcher( DispatchTable );
		return 0;
	}
	int TheMain( int argc,char** argv ){
		DUMP_LEAK;
		if( argc > 1 )
		{
			std::string cmd = argv[1];
			if( cmd == "/install" )
			{
				return CommandInstall( argc,argv );
			}
			else if( cmd == "/uninstall" )
			{
				return CommandUninstall( argc,argv );
			}
			else if( cmd == "/help" )
			{
				return CommandHelp( argc,argv );
			}
			else if( cmd == "/console" )
			{
				return CommandConsole( argc, argv );
			}
			else if( cmd == "/service" )
			{
				return CommandService( argc, argv );
			}
			else{
				printf( "Invalid parameter!\nEnter /help to get more infomation!\n" );
			}
		}else{
			printf( "Invalid parameter!\nEnter /help to get more infomation!\n" );
		}
		return 0;
	}
}
