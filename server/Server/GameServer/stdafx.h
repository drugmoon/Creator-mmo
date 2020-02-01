// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once

#define _WIN32_WINNT 0x0400

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <process.h>
#include <mswsock.h>
#include <shlwapi.h>
//
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <time.h>
//
#include "leudgrid/Performance.h"

#include "leudgrid/EventQueue.h"
#include "leudgrid/MonitorIOCP.h"
#include "leudgrid/Application.h"
#include "leudgrid/ServerMain.h"
#include "leudgrid/EventBase.h"
#include "leudgrid/TCPConnect.h"
#include "leudgrid/TCPAccept.h"
#include "leudgrid/TCPConnectEventQueue.h"
#include "leudgrid/TCPSessionMini.h"

#include "leudgrid/StreamBlock.inl"
#include "leudgrid/StreamFix.h"
#include "leudgrid/StreamBaseEx.h"
#include "leudgrid/DBMysql.h"
//
#include "leudgrid/EventQueueThread.h"

#include "leudgrid/SoftwareKey.h"

#include "leudgrid/Logger.h"
#include "sexy_db_mysql.h"

extern "C" {
#include "7zCrc.h"
#include "7zFile.h"
#include "7zVersion.h"
#include "Archive\7z\7zAlloc.h"
#include "Archive\7z\7zExtract.h"
#include "Archive\7z\7zIn.h"
}

#define LEUD_INT64 _int64