#include "stdafx.h"
#include <DbgHelp.h>
#include <ctime>
#include "leudgrid/DebugCrash.h"

namespace LEUD
{
	typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
		IN HANDLE hProcess,
		IN DWORD ProcessId,
		IN HANDLE hFile,
		IN MINIDUMP_TYPE DumpType,
		IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
		IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
		IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
		);

	LONG WINAPI CrashHandlerFilter(_EXCEPTION_POINTERS *ExceptionInfo ){
		LONG ret = EXCEPTION_CONTINUE_SEARCH;
		HMODULE hDll = ::LoadLibrary("dbghelp.dll");
		if( hDll )
		{
			MINIDUMPWRITEDUMP   pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
			time_t		now;time(&now);
			tm tmnow;
			localtime_s(&tmnow, &now );
			char szFileName[MAX_PATH];
			strftime(szFileName, MAX_PATH, "Exception_%b_%d_%H_%M_%S.dmp", &tmnow);
			HANDLE hFile = ::CreateFile( szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL ); 
			if (hFile!=INVALID_HANDLE_VALUE) 
			{ 
				MINIDUMP_EXCEPTION_INFORMATION ExInfo; 
				ExInfo.ThreadId = ::GetCurrentThreadId(); 
				ExInfo.ExceptionPointers = ExceptionInfo; 
				ExInfo.ClientPointers = NULL; 
				BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithDataSegs, &ExInfo, NULL, NULL ); 
				if (bOK) 
				{
					ret = EXCEPTION_EXECUTE_HANDLER;
				}
				::CloseHandle(hFile); 
			} 
		}
		return ret;
	}

	CrashHandler::CrashHandler(){
		SetUnhandledExceptionFilter(CrashHandlerFilter);
	}
}