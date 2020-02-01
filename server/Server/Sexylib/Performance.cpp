#ifdef _WIN32
#pragma warning(disable:4996)

#include "windows.h"
#include "stdio.h"
#include "leudgrid/Performance.h"
#include "assert.h"
#include "time.h"
#include "limits.h"

static __int64 g_i64CpuFreq = 2000000000;	//默认CPU指令频率
static const int CALC_FREQ_TIME = 500;		//计算cpu频率时间

unsigned int g_dwFileFlag = 'PERF';
unsigned int g_dwFileVersion = '1000';

PerfManager* PerfManager::theOne = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
//pfl文件头
struct PERF_FILE_HEADER
{
	unsigned int	dwFlag;			//文件标识
	unsigned int	dwVersion;		//版本
	__int64			i64CPUFreq;		//cpu主频
	int				iNumNode;		//采样节点总数
	int				iUserDataSize;	//用户数据总数
	unsigned int	dwReserve1;		//备用
	unsigned int	dwReserve2;
	unsigned int	dwReserve3;
	unsigned int	dwReserve4;
};
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//pfl文件采样节点结构
struct PERF_NODE_HEADER
{
	char	szName[MAX_NODE_NAME_LEN];	//节点名称
	int		iParent;				//父节点序号
	int		iSibling;				//下一个兄弟节点序号
	int		iChild;					//子节点序号
};
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//pfl文件采样节点数据
struct PERF_NODE_DATA
{
	int			iTotalCalls;	//总调用次数
	__int64		iTotalTime;		//总开销时间(cpu周期数)
};
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//计算本机CPU指令频率
///////////////////////////////////////////////////////////////////////////////////////////////////
void PerfCountTickRate()
{
	HKEY Key;
	DWORD Speed = 0;
	DWORD Size = sizeof(Speed);

	long Error = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
							   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
							   0, KEY_READ, &Key );
	if ( Error != ERROR_SUCCESS )
	{
		return;
	}
	Error = RegQueryValueEx(Key, "~MHz", NULL, NULL, (LPBYTE)&Speed, &Size);

	RegCloseKey(Key);

	if ( Error != ERROR_SUCCESS )
	{
		return;
	}

	g_i64CpuFreq = Speed * 1000000;

	//int		iTimeBefore, iTimeAfter, iTimeUsed;
	//__int64	iTickBefore, iTickAfter, iTickUsed;

	//iTimeBefore = GetTickCount();
	//PerfGetTicks( &iTickBefore );

	//Sleep( CALC_FREQ_TIME );

	//iTimeAfter = GetTickCount();
	//PerfGetTicks( &iTickAfter );

	//iTimeUsed = iTimeAfter - iTimeBefore;
	//iTickUsed = iTickAfter - iTickBefore;

	//g_i64CpuFreq = iTickUsed * 1000 / iTimeUsed;

	//return g_i64CpuFreq;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//获取CPU指令频率
///////////////////////////////////////////////////////////////////////////////////////////////////
__int64 PerfGetTickRate()
{
	return g_i64CpuFreq;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// CPerfNode
///////////////////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************************
 * INPUT:                                                                                      *
 * name - pointer to a static string which is the name of this profile node                    *
 * parent - parent pointer                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * The name is assumed to be a static pointer, only the pointer is stored and compared for     *
 * efficiency reasons.                                                                         *
 *=============================================================================================*/
//iParent	父节点序号
PerfNode::PerfNode( const char* name, PerfNode* parent ) :
	Name( name ),
	TotalCalls( 0 ),
	TotalTime( 0 ),
	StartTime( 0 ),
	RecursionCounter( 0 ),
	Parent( parent ),
	Child( NULL ),
	Sibling( NULL )
{
	Reset();
}

PerfNode::PerfNode( const char* name ) :
	Name( name ),
	TotalCalls( 0 ),
	TotalTime( 0 ),
	StartTime( 0 ),
	RecursionCounter( 0 ),
	Parent( NULL ),
	Child( NULL ),
	Sibling( NULL )
{
	Reset();
}

PerfNode::~PerfNode()
{
	delete Child;
	delete Sibling;
}

void PerfNode::Reset()
{
	TotalCalls = 0;
	TotalTime = 0;

	if ( Child )
	{
		Child->Reset();
	}
	if ( Sibling )
	{
		Sibling->Reset();
	}
}

/***************************************************************************************************
**
** PerfIterator
**
***************************************************************************************************/
PerfIterator::PerfIterator( PerfNode* start )
{
	CurrentParent = start;
	CurrentChild = CurrentParent->GetChild();
}

PerfNode* PerfIterator::First()
{
	return ( CurrentChild = CurrentParent->GetChild() );
}

PerfNode* PerfIterator::Next()
{
	return ( CurrentChild = CurrentChild->GetSibling() );
}

bool PerfIterator::IsDone()
{
	return CurrentChild == NULL;
}

void PerfIterator::EnterChild( int index )
{
	CurrentChild = CurrentParent->GetChild();
	while ( CurrentChild != NULL && index-- != 0 )
	{
		CurrentChild = CurrentChild->GetSibling();
	}
	if ( CurrentChild != NULL )
	{
		CurrentParent = CurrentChild;
		CurrentChild = CurrentParent->GetChild();
	}
}

void PerfIterator::EnterParent()
{
	if ( CurrentParent->GetParent() != NULL )
	{
		CurrentParent = CurrentParent->GetParent();
	}
	CurrentChild = CurrentParent->GetChild();
}

int PerfIterator::GetTotalChild()
{
	int iNum = 0;
	PerfNode *pChild = CurrentParent->GetChild();
	while ( pChild != NULL )
	{
		iNum++;
		pChild = pChild->GetSibling();
	}
	return iNum;
}

/***************************************************************************************************
**
** CPerfManager
**
***************************************************************************************************/

PerfManager::PerfManager()
{
	PerfCountTickRate();

	m_pRoot = new PerfNode( "Root", NULL );
	m_pRoot->iIndex = 0;
	m_iTotalNode = 1;
	m_pCurrentNode = m_pRoot;
	m_pNodeArr = NULL;	//节点指针数组, 从文件中批量创建节点时使用
	m_pNodeData = NULL;	//存放节点名字的缓冲区指针
	FrameCounter = 0;
	ResetTime = 0;
	m_pRoot->Reset();
}

PerfManager::~PerfManager()
{
	ReleaseMemory();
}

PerfNode* PerfManager::CreateNewNode( const char* name, PerfNode *pParent )
{
	PerfNode *pNode = new PerfNode( name, pParent );
	pNode->iIndex = m_iTotalNode++;
	pNode->Sibling = pParent->GetChild();
	pParent->Child = pNode;
	return pNode;
}

/***********************************************************************************************
 * PerfManager::Reset -- Reset the contents of the profiling system						   *
 *                                                                                             *
 *    This resets everything except for the tree structure.  All of the timing data is reset.  *
 *=============================================================================================*/
void PerfManager::Reset()
{ 
	m_pRoot->Reset(); 
	FrameCounter = 0;
	PerfGetTicks( &ResetTime );
}

/***********************************************************************************************
 * PerfManager::Increment_Frame_Counter -- Increment the frame counter						   *
 *=============================================================================================*/
void PerfManager::IncrementFrameCounter()
{
	FrameCounter++;
}

/***********************************************************************************************
 * PerfManager::GetTime_Since_Reset -- returns the elapsed time since last reset			   *
 *=============================================================================================*/
__int64 PerfManager::GetTimeSinceReset()
{
	__int64 time;
	PerfGetTicks( &time );
	time -= ResetTime;
	return time;
}

///////////////////////////////////////////////////////////////////////////////
//创建存盘文件头
//pPath			存盘文件路径
//iUserDataSize	用户统计数据的大小(int个数)
//pUserDataName	用户统计数据的名字数组, 如果iUserDataSize不为0则此参数不能为NULL
//				用户数据名必须小于64个字符
//return		成功返回true, 失败返回 false
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::CreateFileHeader( const char* pPath, int iUserDataSize, const char** pUserDataName )
{
	if ( NULL == pPath )
	{
		return false;
	}
	if ( iUserDataSize > 0 && NULL == pUserDataName )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "wb" );
	if ( NULL == fp )
	{
		return false;
	}
	
	PERF_FILE_HEADER fh;

	fh.dwFlag = g_dwFileFlag;
	fh.dwVersion = g_dwFileVersion;
	fh.i64CPUFreq = g_i64CpuFreq;
	fh.iNumNode = m_iTotalNode;
	fh.iUserDataSize = iUserDataSize;
	fh.dwReserve1 = 0;
	fh.dwReserve2 = 0;
	fh.dwReserve3 = 0;
	fh.dwReserve4 = 0;

	if ( fwrite( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	
	int iBufferSize = sizeof( PERF_NODE_HEADER ) * m_iTotalNode
					+ MAX_NODE_NAME_LEN * iUserDataSize;
	//将所有节点名字写入
	BYTE *pBuffer = new BYTE[iBufferSize];
	memset( pBuffer, 0, iBufferSize );
	ExportNodeHeader( m_pRoot, (PERF_NODE_HEADER*)pBuffer );
	//用户数据名也写入文件头
	char *pTemp = (char*)pBuffer + sizeof( PERF_NODE_HEADER ) * m_iTotalNode;
	for ( int i = 0; i < iUserDataSize; i++ )
	{
		if ( pUserDataName[i] != NULL )
		{
			assert( strlen( pUserDataName[i] ) < MAX_NODE_NAME_LEN );
			strcpy( pTemp, pUserDataName[i] );
		}
		pTemp += MAX_NODE_NAME_LEN;
	}

	bool ret = true;
	if ( fwrite( pBuffer, iBufferSize, 1, fp ) != 1 )
	{
		ret = false;
	}
	delete[] pBuffer;
	fclose( fp );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
//当有新的采样节点被调用时，由于节点数变化，必须重建pfl文件
bool PerfManager::RecreateDataFile( const char* pPath,
									 int iTotalNodeOld,
									 int iUserDataSize,
									 const char** pUserDataName
									)
{
	assert( m_iTotalNode > iTotalNodeOld );
	//备份一下
	char szBakFilename[MAX_PATH];
	sprintf( szBakFilename, "%s.bak.pfl", pPath );
	CopyFile( pPath, szBakFilename, false );	//如果文件存在则覆盖
	//删除
	DeleteFile( pPath );

	//创建新文件头
	if ( !CreateFileHeader( pPath, iUserDataSize, pUserDataName ) )
	{
		return false;
	}
	FILE *fpOld = fopen( szBakFilename, "rb" );
	if ( NULL == fpOld )
	{
		return false;
	}
	FILE *fpNew = fopen( pPath, "ab" );
	if ( NULL == fpNew )
	{
		fclose( fpOld );
		return false;
	}
	//复制原文件的内容
	//帧尺寸
	int iFrameSizeOld	= sizeof( int ) * iUserDataSize		//用户数据
						+ sizeof( int )						//采样帧数
						+ sizeof( time_t )					//时间（1970年来的秒数）
						+ sizeof( PERF_NODE_DATA ) * iTotalNodeOld;	//采样数据
	int iFrameSizeNew	= sizeof( int ) * iUserDataSize		//用户数据
						+ sizeof( int )						//采样帧数
						+ sizeof( time_t )					//时间（1970年来的秒数）
						+ sizeof( PERF_NODE_DATA ) * m_iTotalNode;	//采样数据
	assert( iFrameSizeNew > iFrameSizeOld );
	//第一帧数据偏移
	int iPosOld	= sizeof( PERF_FILE_HEADER )		//文件头
				+ sizeof( PERF_NODE_HEADER ) * iTotalNodeOld	//采样节点头
				+ MAX_NODE_NAME_LEN * iUserDataSize;		//用户数据名
	int iPosNew	= sizeof( PERF_FILE_HEADER )		//文件头
				+ sizeof( PERF_NODE_HEADER ) * m_iTotalNode	//采样节点头
				+ MAX_NODE_NAME_LEN * iUserDataSize;		//用户数据名
	
	BYTE *pBuffer = new BYTE[iFrameSizeNew];
	memset( pBuffer, 0, iFrameSizeNew );
	while ( fseek( fpOld, iPosOld, SEEK_SET ) == 0 )
	{
		if ( fseek( fpNew, iPosNew, SEEK_SET ) != 0 )
		{
			break;
		}
		//如果是360平台，原来写的时候字节序已经是little-endian了，所以这里什么也不用干
		if ( fread( pBuffer, iFrameSizeOld, 1, fpOld ) != 1 )
		{
			break;
		}
		if ( fwrite( pBuffer, iFrameSizeNew, 1, fpNew ) != 1 )
		{
			break;
		}
		iPosOld += iFrameSizeOld;
		iPosNew += iFrameSizeNew;
	}
	delete[] pBuffer;
	fclose( fpOld );
	fclose( fpNew );

	DeleteFile( szBakFilename );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//单个节点结构导出
//pNode		导出节点
//pHeader	导出缓冲区
///////////////////////////////////////////////////////////////////////////////
void PerfManager::ExportNodeHeader( PerfNode *pNode, PERF_NODE_HEADER* pHeader )
{
	assert( NULL != pNode );
	assert( NULL != pHeader );
	
	int iIndex = pNode->GetIndex();
	strcpy( pHeader[iIndex].szName, pNode->GetName() );
	if ( pNode->GetParent() != NULL )
	{
		pHeader[iIndex].iParent = pNode->GetParent()->GetIndex();
	}
	else
	{
		//嘿嘿-1不用swap
		pHeader[iIndex].iParent = -1;
	}
	if ( pNode->GetChild() != NULL )
	{
		pHeader[iIndex].iChild = pNode->GetChild()->GetIndex();
		ExportNodeHeader( pNode->GetChild(), pHeader );
	}
	else
	{
		pHeader[iIndex].iChild = -1;
	}
	if ( pNode->GetSibling() != NULL )
	{
		pHeader[iIndex].iSibling = pNode->GetSibling()->GetIndex();
		ExportNodeHeader( pNode->GetSibling(), pHeader );
	}
	else
	{
		pHeader[iIndex].iSibling = -1;
	}
}

///////////////////////////////////////////////////////////////////////////////
//单个节点数据导出
//pNode		导出节点
//pData		导出缓冲区
///////////////////////////////////////////////////////////////////////////////
void PerfManager::ExportNodeData( PerfNode *pNode, PERF_NODE_DATA* pData )
{
	assert( pNode != NULL );
	assert( pData != NULL );

	int iIndex = pNode->GetIndex();
	//放在缓冲区的iIndex位置

	pData[iIndex].iTotalCalls = pNode->GetTotalCalls();
	pData[iIndex].iTotalTime = pNode->GetTotalTime();
	if ( pNode->GetChild() != NULL )
	{
		ExportNodeData( pNode->GetChild(), pData );
	}
	if ( pNode->GetSibling() != NULL )
	{
		ExportNodeData( pNode->GetSibling(), pData );
	}
}

///////////////////////////////////////////////////////////////////////////////
//将当前所有采样节点的数据写入文件
//pPath			存盘文件路径
//pUserData		需要一同写入文件的用户统计数据数组指针, 如果为空则代表没有用户数据
//iSize			pUserData指向的int数组中的数据个数(对于同一个文件,每次调用必须相同)
//pUserDataName	用户统计数据的名字数组(对于同一个文件,每次调用必须相同)
//				如果pUserData不为NULL, 则这个参数也不能为NULL
//return		成功返回true, 失败返回 false

//文件结构
//	int			采样节点数
//	int			用户数据个数
//	PERF_NODE_HEADER		采样节点1	由于每个采样数据帧都有同样的采样节点
//										而且它们的相互调用关系也是固定的
//										所以把采样节点的名字和相互关系单独提取出来
//										放在文件头里
//	PERF_NODE_HEADER		采样节点2
//	...			
//	char[64]	用户数据名1
//	char[64]	用户数据名2
//	...
//	frame1		采样数据帧1
//	frame2		采样数据帧2
//	...

//采样数据帧结构
//	userdata	用户数据
//	int			采样帧数(是指游戏中的帧, 不是前面说的数据帧)
//	PERF_NODE_DATA	节点数据1
//	PERF_NODE_DATA	节点数据2
//	...

//注意:		如果当前采样节点数比文件中记载的要多, 则文件会被重建, 之前的采样数据全部丢失
//			因为所有采样节点的名字是记录在文件头中的, 如果节点数不一致
//			则节点名和节点数据就无法对应了, 只能重新创建文件

///////////////////////////////////////////////////////////////////////////////
bool PerfManager::SaveDataFrame( const char* pPath, int* pUserData, const char** pUserDataName, int iUserDataSize )
{
	if ( NULL == pPath )
	{
		return false;
	}

	if ( !HandleOldFile( pPath, iUserDataSize, pUserDataName ) )
	{
		return false;
	}

	FILE *fp = fopen( pPath, "ab" );
	if ( NULL == fp )
	{
		return false;
	}
	if ( NULL == pUserData )
	{
		iUserDataSize = 0;
	}

	int iBufferSize = sizeof( int ) * iUserDataSize	//用户数据
					+ sizeof( int )			//采样帧数
					+ sizeof( time_t )		//时间（1970年来的秒数）
					+ sizeof( PERF_NODE_DATA ) * m_iTotalNode;	//节点数据
	BYTE *pBuffer = new BYTE[iBufferSize];
	BYTE *pCur = pBuffer;
	//用户数据
	if ( pUserData != NULL )
	{
		memcpy( pCur, pUserData, sizeof( int ) * iUserDataSize );
	}
	pCur += ( sizeof( int ) * iUserDataSize );
	//采样帧数
	*((int*)pCur) = GetFrameCountSinceReset();
	pCur += sizeof( int );

	time_t Time;
	time( &Time );

	*((time_t*)pCur) = Time;

	pCur += sizeof( time_t );

	ExportNodeData( m_pRoot, (PERF_NODE_DATA*)pCur );

	//一次性将整个buffer写入文件
	bool ret = true;
	if ( fwrite( pBuffer, iBufferSize, 1, fp ) != 1 )
	{
		ret = false;
	}
	delete[] pBuffer;

	fclose( fp );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
//以下函数为PerfSpy专用
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//打开pfl数据文件
//pPath				文件路径
//pTotalDataFrame	返回数据帧个数
//pUserDataSize		返回用户数据个数
//return			成功返回true, 失败返回false 
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::OpenDataFile( const char* pPath, int* pTotalDataFrame, int* pUserDataSize )
{
	if ( NULL == pPath )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	if ( !VerifyFileHeader( &fh ) )
	{
		return false;
	}

	if ( pUserDataSize != NULL )
	{
		*pUserDataSize = fh.iUserDataSize;
	}
	g_i64CpuFreq = fh.i64CPUFreq;

	//以下为读取采样节点的名字并组织树状结构
	if ( m_pNodeData != NULL )
	{
		delete[] m_pNodeData;
	}
	//保存节点的名字和相互关系的缓冲区
	m_pNodeData = new PERF_NODE_HEADER[fh.iNumNode];

	if ( fread( m_pNodeData, sizeof( PERF_NODE_HEADER ) * fh.iNumNode, 1, fp ) != 1 )
	{
		delete[] m_pNodeData;
		m_pNodeData = NULL;
		fclose( fp );
		return false;
	}

	if ( m_pNodeArr != NULL )
	{
		delete[] m_pNodeArr;
	}
	m_pNodeArr = new PerfNode*[fh.iNumNode];
	memset( m_pNodeArr, 0, fh.iNumNode * sizeof( PerfNode* ) );
	//删除旧的节点
	if ( m_pRoot->GetChild() )
	{
		//m_pRoot不会有兄弟, 只删除儿子就行了
		delete m_pRoot->GetChild();
		m_pRoot->Child = NULL;
	}
	m_iTotalNode = 1;	//还剩一个m_pRoot, 所以是1
	for ( int i = 1; i < fh.iNumNode; i++ )	//第0个节点是m_pRoot, 不用再创建
	{
		m_pNodeArr[i] = new PerfNode( m_pNodeData[i].szName );
		m_pNodeArr[i]->iIndex = i;
	}
	m_iTotalNode = fh.iNumNode;
	//组织关系
	for ( int i = 1; i < fh.iNumNode; i++ )
	{
		if ( m_pNodeData[i].iChild >= 0 )
		{
			m_pNodeArr[i]->Child = m_pNodeArr[m_pNodeData[i].iChild];
		}
		if ( m_pNodeData[i].iSibling >= 0 )
		{
			m_pNodeArr[i]->Sibling = m_pNodeArr[m_pNodeData[i].iSibling];
		}
		if ( 0 == m_pNodeData[i].iParent )
		{
			//父亲是根节点
			m_pNodeArr[i]->Parent = m_pRoot;
		}
		else
		{
			//父亲不是根节点
			m_pNodeArr[i]->Parent = m_pNodeArr[m_pNodeData[i].iParent];
		}
	}
	if ( fh.iNumNode > 1 )
	{
		m_pRoot->Child = m_pNodeArr[m_pNodeData[0].iChild];
	}

	//一下统计数据帧数
	int iFrameSize	= sizeof( int ) * fh.iUserDataSize	//用户数据
					+ sizeof( int )		//采样帧数
					+ sizeof( time_t )	//时间（1970年来的秒数）
					+ sizeof( PERF_NODE_DATA ) * fh.iNumNode;	//采样数据

	int iHeaderSize = sizeof( PERF_FILE_HEADER )		//文件头
					+ sizeof( PERF_NODE_HEADER ) * fh.iNumNode	//采样节点头
					+ MAX_NODE_NAME_LEN * fh.iUserDataSize;	//用户数据名
	//文件大小
	if ( fseek( fp, 0, SEEK_END ) != 0 )
	{
		fclose( fp );
		return false;
	}
	fpos_t pos;
	if ( fgetpos( fp, &pos ) != 0 )
	{
		fclose( fp );
		return false;
	}

	if ( pTotalDataFrame != NULL )
	{
		*pTotalDataFrame = (int)( pos - iHeaderSize ) / iFrameSize;
	}

	fclose( fp );

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//从指定文件中读出一个采样数据帧
//pPath			存盘文件路径
//iFrameIndex	读文件中的第几个数据帧
//pUserData		用来接收用户数据的缓冲区, 该缓冲区必须足够大否则会产生致命错误
//				为NULL则不接收用户数据
//pTime			保存该数据帧时的时间
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::LoadDataFrame( const char* pPath, int iFrameIndex, int* pUserData, time_t *pTime )
{
	if ( NULL == pPath )
	{
		return false;
	}
	if ( NULL == m_pNodeArr || NULL == m_pNodeData )
	{
		assert( false );
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	if ( fh.iNumNode != m_iTotalNode )
	{
		fclose( fp );
		return false;
	}
	//采样数据帧大小
	int iFrameSize	= sizeof( int ) * fh.iUserDataSize	//用户数据
					+ sizeof( int )						//采样帧数
					+ sizeof( time_t )					//时间（1970年来的秒数）
					+ sizeof( PERF_NODE_DATA ) * fh.iNumNode;	//采样数据
	//文件偏移
	int iOffset = sizeof( PERF_FILE_HEADER )		//文件头
				+ sizeof( PERF_NODE_HEADER ) * fh.iNumNode	//采样节点头
				+ MAX_NODE_NAME_LEN * fh.iUserDataSize	//用户数据名
				+ iFrameSize * iFrameIndex;

	if ( fseek( fp, iOffset, SEEK_SET ) != 0 )
	{
		fclose( fp );
		return false;
	}
	if ( pUserData != NULL )
	{
		if ( fread( pUserData, sizeof( int ) * fh.iUserDataSize, 1, fp ) != 1 )
		{
			fclose( fp );
			return false;
		}
	}
	else
	{
		if ( fseek( fp, sizeof( int ) * fh.iUserDataSize, SEEK_CUR ) != 0 )
		{
			fclose( fp );
			return false;
		}
	}
	//采样帧数
	int iCount;
	if ( fread( &iCount, sizeof( int ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	FrameCounter = iCount;

	//时间
	time_t time;
	if ( fread( &time, sizeof( time_t ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	if ( pTime != NULL )
	{
		*pTime = time;
	}

	//采样节点数据
	PERF_NODE_DATA *pBuffer = new PERF_NODE_DATA[fh.iNumNode];
	if ( fread( pBuffer, fh.iNumNode * sizeof( PERF_NODE_DATA ), 1, fp ) != 1 )
	{
		delete[] pBuffer;
		fclose( fp );
		return false;
	}
	for ( int i = 1; i < fh.iNumNode; i++ )
	{
		m_pNodeArr[i]->TotalCalls	= pBuffer[i].iTotalCalls;
		m_pNodeArr[i]->TotalTime	= pBuffer[i].iTotalTime;
	}
	delete[] pBuffer;
	fclose( fp );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//获取指定文件中的用户数据名
//pPath				文件路径
//pName				用户保存用户名的缓冲区
//					该缓冲区尺寸必须大于 用户数据个数 x MAX_NODE_NAME_LEN
//return			成功返回true, 失败返回false 
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::GetUserDataName( const char* pPath, char* pName )
{
	if ( NULL == pPath || NULL == pName )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	int iPos = sizeof( PERF_FILE_HEADER )
			 + sizeof( PERF_NODE_HEADER ) * fh.iNumNode;
	if ( fseek( fp, iPos, SEEK_SET ) != 0 )
	{
		fclose( fp );
		return false;
	}
	char *pBuffer = new char[MAX_NODE_NAME_LEN * fh.iUserDataSize];
	if ( fread( pBuffer, MAX_NODE_NAME_LEN * fh.iUserDataSize, 1, fp ) != 1 )
	{
		delete[] pBuffer;
		fclose( fp );
		return false;
	}
	char *pSrc = pBuffer,
		 *pDst = pName;
	for ( int i = 0; i < fh.iUserDataSize; i++ )
	{
		assert( strlen( pSrc ) < MAX_NODE_NAME_LEN );
		strcpy( pDst, pSrc );
		pSrc += MAX_NODE_NAME_LEN;
		pDst += MAX_NODE_NAME_LEN;
	}
	delete[] pBuffer;
	fclose( fp );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//从指定文件中获取指定节点在所有数据帧的耗时数组
//pPath				文件路径
//iNodeIndex		指定节点序号
//pTime				总耗时数组缓冲区，该缓冲区必须足够大
//pTimePerFr		每帧耗时数组缓冲区，该缓冲区必须足够大
//iMaxTime			返回耗时最大值
//iMinTime			返回耗时最小值
//iMaxIndex			返回最大值对应的数据帧序号
//iMinIndex			返回最小值对应的数据帧序号
//iMaxTimePerFr		返回最大每帧耗时
//return			成功返回true, 失败返回false
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::GetNodeTimeArr( const char* pPath, int iNodeIndex,
									__int64* pTime, __int64* pTimePerFr,
									__int64& iMaxTime, __int64& iMinTime,
									int& iMaxIndex, int& iMinIndex, __int64& iMaxTimePerFr )
{
	if ( NULL == pPath || NULL == pTime || NULL == pTimePerFr )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	//iTotalTime成员地址到结构指针的偏移
	PERF_NODE_DATA temp;
	int iTimeOffset = int( ((BYTE*)&(temp.iTotalTime)) - (BYTE*)(&temp) );

	//int iTimeOffset = (int) ( &( ((PERF_NODE_DATA*)( NULL ))->iTotalTime ) );
	
	//第一个数据帧的帧数数据偏移
	int iPosFr	= sizeof( PERF_FILE_HEADER )
				+ sizeof( PERF_NODE_HEADER ) * fh.iNumNode
				+ MAX_NODE_NAME_LEN * fh.iUserDataSize
				+ sizeof( int ) * fh.iUserDataSize;	//用户数据
	//第一个数据帧的指定节点的数据偏移
	int iPos = sizeof( PERF_FILE_HEADER )
		     + sizeof( PERF_NODE_HEADER ) * fh.iNumNode
		     + MAX_NODE_NAME_LEN * fh.iUserDataSize
		     + sizeof( int ) * fh.iUserDataSize	//用户数据
		     + sizeof( int )					//采样帧数
			 + sizeof( time_t )					//时间（1970年来的秒数）
		     + sizeof( PERF_NODE_DATA ) * iNodeIndex
		     //+ sizeof( int );		//跳过iTotalCall
			 + iTimeOffset;

	//帧大小
	int iFrameSize = sizeof( int ) * fh.iUserDataSize
				   + sizeof( int )
				   + sizeof( time_t )		//时间（1970年来的秒数）
				   + sizeof( PERF_NODE_DATA ) * fh.iNumNode;
				
	iMaxTime = _I64_MIN;
	iMaxIndex = 0;
	iMinTime = _I64_MAX;
	iMinIndex = 0;
	iMaxTimePerFr = _I64_MIN;
	int iDataFrameCount = 0;
	int iTotalFrame = 0;
	while ( true )
	{
		if ( fseek( fp, iPosFr, SEEK_SET ) != 0 )
		{
			break;
		}
		if ( fread( &iTotalFrame, sizeof( int ), 1, fp ) != 1 )
		{
			break;
		}
		if ( fseek( fp, iPos, SEEK_SET ) != 0 )
		{
			break;
		}
		if ( fread( pTime, sizeof( __int64 ), 1, fp ) != 1 )
		{
			break;
		}
		if ( *pTime > iMaxTime )
		{
			iMaxTime = *pTime;
			iMaxIndex = iDataFrameCount;
		}
		if ( *pTime < iMinTime )
		{
			iMinTime = *pTime;
			iMinIndex = iDataFrameCount;
		}
		if ( iTotalFrame > 0 )
		{
			*pTimePerFr = *pTime / iTotalFrame;
		}
		else
		{
			*pTimePerFr = 0;
		}
		if ( *pTimePerFr > iMaxTimePerFr )
		{
			iMaxTimePerFr = *pTimePerFr;
		}
		iPos += iFrameSize;
		iPosFr += iFrameSize;
		iDataFrameCount++;
		pTime++;
		pTimePerFr++;
	}
	fclose( fp );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//从指定文件中获取所有数据帧中的指定用户数据
//pPath				文件路径
//iIndex			指定用户数据序号
//pData				接收数据缓冲区, 该缓冲区必须足够大
//iMaxValue			返回用户数据最大值
//iMinValue			返回用户数据最小值
//iMaxIndex			返回最大值对应的数据帧序号
//iMinIndex			返回最小值对应的数据帧序号
//return			成功返回true, 失败返回false
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::GetUserDataArr( const char* pPath, int iIndex, int* pData,
								int& iMaxValue, int& iMinValue,
								int& iMaxIndex, int& iMinIndex )
{
	if ( NULL == pPath || NULL == pData )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	//第一个数据帧的指定用户数据的数据偏移
	int iPos = sizeof( PERF_FILE_HEADER )
		     + sizeof( PERF_NODE_HEADER ) * fh.iNumNode
		     + MAX_NODE_NAME_LEN * fh.iUserDataSize
		     + sizeof( int ) * iIndex;

	//帧大小
	int iFrameSize = sizeof( int ) * fh.iUserDataSize
				   + sizeof( int )
				   + sizeof( time_t )		//时间（1970年来的秒数）
				   + sizeof( PERF_NODE_DATA ) * fh.iNumNode;

	iMaxValue = INT_MIN;
	iMinValue = INT_MAX;
	iMaxIndex = 0;
	iMinIndex = 0;
	int	iDataFrameCount = 0;
	while ( true )
	{
		if ( fseek( fp, iPos, SEEK_SET ) != 0 )
		{
			break;
		}
		if ( fread( pData, sizeof( int ), 1, fp ) != 1 )
		{
			break;
		}
		if ( *pData > iMaxValue )
		{
			iMaxValue = *pData;
			iMaxIndex = iDataFrameCount;
		}
		if ( *pData < iMinValue )
		{
			iMinValue = *pData;
			iMinIndex = iDataFrameCount;
		}
		iPos += iFrameSize;
		iDataFrameCount++;
		pData++;
	}
	fclose( fp );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//从指定文件中获取指定范围数据帧中的帧数总和
//pPath				文件路径
//iTotalFrame		返回逻辑帧数总和
//iFrameBegin		参与统计的数据帧序号范围
//iFrameEnd			
//return			成功返回true, 失败返回false
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::GetTotalFrame( const char* pPath, int& iTotalFrame,
									int iFrameBegin, int iFrameEnd )
{
	if ( NULL == pPath )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	//第一个数据帧的指定节点的数据偏移
	int iPos = sizeof( PERF_FILE_HEADER )
		     + sizeof( PERF_NODE_HEADER ) * fh.iNumNode
		     + MAX_NODE_NAME_LEN * fh.iUserDataSize
		     + sizeof( int ) * fh.iUserDataSize;//用户数据
	//帧大小
	int iFrameSize = sizeof( int ) * fh.iUserDataSize
				   + sizeof( int )
				   + sizeof( time_t )		//时间（1970年来的秒数）
				   + sizeof( PERF_NODE_DATA ) * fh.iNumNode;

	int iDataFrameIndex = 0;			
	iTotalFrame = 0;
	int iFrame;
	while ( true )
	{
		if ( fseek( fp, iPos, SEEK_SET ) != 0 )
		{
			break;
		}
		if ( iDataFrameIndex >= iFrameEnd )
		{
			break;
		}
		//只统计iFrameBegin和iFrameEnd之间的数据
		if ( iDataFrameIndex >= iFrameBegin )
		{
			if ( fread( &iFrame, sizeof( int ), 1, fp ) != 1 )
			{
				break;
			}
			iTotalFrame += iFrame;
		}
		iPos += iFrameSize;
		iDataFrameIndex++;
	}
	fclose( fp );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//从指定文件中获取所有数据帧中的数据总和
//pPath				文件路径
//iIndex			指定节点序号
//i64TotalTime		指定节点在所有数据帧的耗时总和
//iTotalCall		指定节点在所有数据帧的调用次数总和
//iFrameBegin		参与统计的数据帧序号范围
//iFrameEnd			
//return			成功返回true, 失败返回false
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::GetNodeTotalData( const char* pPath, int iNodeIndex,
									 __int64& i64TotalTime, int& iTotalCall,
									 int iFrameBegin, int iFrameEnd )
{
	if ( NULL == pPath )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	//第一个数据帧的指定节点的数据偏移
	int iPos = sizeof( PERF_FILE_HEADER )
		     + sizeof( PERF_NODE_HEADER ) * fh.iNumNode
		     + MAX_NODE_NAME_LEN * fh.iUserDataSize
		     + sizeof( int ) * fh.iUserDataSize	//用户数据
		     + sizeof( int )					//采样帧数
			 + sizeof( time_t )					//时间（1970年来的秒数）
		     + sizeof( PERF_NODE_DATA ) * iNodeIndex;

	//帧大小
	int iFrameSize = sizeof( int ) * fh.iUserDataSize
				   + sizeof( int )
				   + sizeof( time_t )		//时间（1970年来的秒数）
				   + sizeof( PERF_NODE_DATA ) * fh.iNumNode;
				
	i64TotalTime = 0;
	iTotalCall = 0;
	PERF_NODE_DATA data;
	int iFrame = 0;
	while ( true )
	{
		if ( fseek( fp, iPos, SEEK_SET ) != 0 )
		{
			break;
		}
		if ( iFrame >= iFrameEnd )
		{
			break;
		}
		//只统计iFrameBegin和iFrameEnd之间的数据
		if ( iFrame >= iFrameBegin )
		{
			if ( fread( &data, sizeof( PERF_NODE_DATA ), 1, fp ) != 1 )
			{
				break;
			}
			i64TotalTime += data.iTotalTime;
			iTotalCall	 += data.iTotalCalls;
		}
		iPos += iFrameSize;
		iFrame++;
	}
	fclose( fp );
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//从指定文件中获取各项用户数据的在所有数据帧中的总和
//pPath				文件路径
//pTotalUserData	返回所有用户数据总和的缓冲区, 该缓冲区必须足够大
//iFrameBegin		参与统计的数据帧序号范围
//iFrameEnd			
//return			成功返回true, 失败返回false
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::GetTotalUserDataArr( const char* pPath, __int64* pTotalUserData,
										  int iFrameBegin, int iFrameEnd )
{
	if ( NULL == pPath || NULL == pTotalUserData )
	{
		return false;
	}
	FILE *fp = fopen( pPath, "rb" );
	if ( NULL == fp )
	{
		return false;
	}
	PERF_FILE_HEADER fh;
	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		return false;
	}
	//第一个数据帧的第一个用户数据的偏移
	int iPos = sizeof( PERF_FILE_HEADER )
		     + sizeof( PERF_NODE_HEADER ) * fh.iNumNode
		     + MAX_NODE_NAME_LEN * fh.iUserDataSize;
	//帧大小
	int iFrameSize = sizeof( int ) * fh.iUserDataSize
				   + sizeof( int )
				   + sizeof( time_t )		//时间（1970年来的秒数）
				   + sizeof( PERF_NODE_DATA ) * fh.iNumNode;

	//清0
	memset( pTotalUserData, 0, sizeof( __int64 ) * fh.iUserDataSize );

	int *pUserData = new int[fh.iUserDataSize];
	int iFrame = 0;
	while ( true )
	{
		if ( fseek( fp, iPos, SEEK_SET ) != 0 )
		{
			break;
		}
		if ( iFrame >= iFrameEnd )
		{
			break;
		}
		//只统计iFrameBegin和iFrameEnd之间的数据
		if ( iFrame >= iFrameBegin )
		{
			if ( fread( pUserData, sizeof( int ) * fh.iUserDataSize, 1, fp ) != 1 )
			{
				break;
			}
			for ( int i = 0; i < fh.iUserDataSize; i++ )
			{
				pTotalUserData[i] += pUserData[i];
			}
		}
		iPos += iFrameSize;
		iFrame++;
	}
	fclose( fp );
	delete[] pUserData;
	return true;
}
	
///////////////////////////////////////////////////////////////////////////////
void PerfManager::ReleaseMemory()
{
	if ( m_pNodeArr != NULL )
	{
		delete[] m_pNodeArr;
		m_pNodeArr = NULL;
	}
	if ( m_pNodeData != NULL )
	{
		delete[] m_pNodeData;
		m_pNodeData = NULL;
	}
	if ( m_pRoot != NULL )
	{
		delete m_pRoot;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool PerfManager::VerifyFileHeader( PERF_FILE_HEADER * pHeader )
{
	assert( pHeader != NULL );

	return ( pHeader->dwFlag == g_dwFileFlag
			&& pHeader->dwVersion == g_dwFileVersion );
}

///////////////////////////////////////////////////////////////////////////////
//处理可能存在的旧数据文件
///////////////////////////////////////////////////////////////////////////////
bool PerfManager::HandleOldFile( const char* pPath, int iUserDataSize, const char** pUserDataName )
{
	PERF_FILE_HEADER fh;

	FILE *fp = fopen( pPath, "r" );
	if ( NULL == fp )
	{
		goto NewFile;
	}

	if ( fread( &fh, sizeof( PERF_FILE_HEADER ), 1, fp ) != 1 )
	{
		fclose( fp );
		goto NewFile;
	}

	int iOldUserDataSize = fh.iUserDataSize;
	int iOldNumNode = fh.iNumNode;

	fclose( fp );
	if ( iOldUserDataSize != iUserDataSize )
	{	//用户数据尺寸变了
		goto NewFile;
	}

	if ( iOldNumNode == m_iTotalNode )
	{	//节点数没变，啥也不干
		return true;
	}

	if ( iOldNumNode > m_iTotalNode )
	{	//节点数少了，抛弃原文件
		goto NewFile;
	}

	//节点数增加了，修复原来的文件
	return RecreateDataFile( pPath, iOldNumNode, iUserDataSize, pUserDataName );

NewFile:
	return CreateFileHeader( pPath, iUserDataSize, pUserDataName );
}

///////////////////////////////////////////////////////////////////////////////
//获取根节点总用时
//根节点用时是它的所有子节点用时的和
///////////////////////////////////////////////////////////////////////////////
__int64 PerfManager::GetRootTotalTime()
{
	__int64 i64Total = 0;
	PerfNode *pNode = m_pRoot->GetChild();

	while ( pNode != NULL )
	{
		i64Total += pNode->GetTotalTime();
		pNode = pNode->GetSibling();
	}
	return i64Total;
}

PerfNode* PerfManager::GetNode( int iIndex )
{
	if ( iIndex < 0 || iIndex >= m_iTotalNode || NULL == m_pNodeArr )
	{
		return NULL;
	}
	return m_pNodeArr[iIndex];
}


 
#endif