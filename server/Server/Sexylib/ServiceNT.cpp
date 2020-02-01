#include "stdafx.h"
#include "leudgrid/ServiceNT.h"

namespace LEUD
{
	bool InstallService( const char* service_name,const char* service_desc,const char* command ){
		bool result = false;
		SC_HANDLE schSCManager = OpenSCManager( NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if (schSCManager) 
		{
			SC_HANDLE schService = CreateService( schSCManager,service_name,service_desc,
				SERVICE_ALL_ACCESS,SERVICE_WIN32_OWN_PROCESS,SERVICE_AUTO_START,SERVICE_ERROR_IGNORE,
				command,NULL,NULL,NULL,NULL,NULL);
			if (schService) 
			{
				CloseServiceHandle(schService); 
				result = true;
			}
			CloseServiceHandle(schSCManager);
		}
		return result;
	}
	bool UninstallService( const char* service_name ){
		bool result = false;
		SC_HANDLE schSCManager = OpenSCManager( NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if (schSCManager)
		{
			SC_HANDLE schService = OpenService( schSCManager,service_name,DELETE);
			if (schService) 
			{
				if ( DeleteService(schService) ) 
				{
					result = true;
				}
				CloseServiceHandle(schService); 
			}
			CloseServiceHandle(schSCManager);
		}
		return result;
	}
}
