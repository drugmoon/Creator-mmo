#pragma once
#include "mysql/mysql.h"
namespace LEUD
{
	class DBModuleMysql
	{
	public:
		std::string m_ip;	
		std::string m_dbname;
		std::string m_user;
		std::string m_pwd;
		size_t m_QueryCount;
		size_t m_QueryResCount;
		size_t m_QueryResFreeCount;
		MYSQL* m_mysql;
	public:
		DBModuleMysql();
		bool Create( std::string ip,std::string port,std::string dbname,std::string user,std::string pwd );
		void Destroy( );
	public:
		unsigned long EscapeString( char* to, const char* from , unsigned long length );		
		MYSQL_RES* QueryUse( const char * sql,bool noret=false,my_ulonglong* effect=NULL );
		MYSQL_RES* QueryStore( const char * sql,bool noret=false,my_ulonglong* effect=NULL );
		void FreeResult( MYSQL_RES* result);
		MYSQL_ROW FetchRow(MYSQL_RES* result);
		int FindFieldPos(MYSQL_RES* result,const char* name);
		void Ping();
	public:
		const char* FetchField(MYSQL_ROW row,MYSQL_RES* result,const char* FieldName,int &length);
	};
}
