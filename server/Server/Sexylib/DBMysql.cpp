#include "stdafx.h"
#include "leudgrid/DBMysql.h"
namespace LEUD
{
	DBModuleMysql::DBModuleMysql(){
		m_mysql = 0;
		m_QueryCount = 0;
		m_QueryResCount = 0;
		m_QueryResFreeCount = 0;
	}
	bool DBModuleMysql::Create( std::string ip,std::string port,std::string dbname,std::string user,std::string pwd )
	{
		m_ip = ip;
		m_dbname = dbname;
		m_user = user;
		m_pwd = pwd;
		m_mysql = mysql_init(NULL);
		if( ! m_mysql )
		{
			return false;
		}
		mysql_options(m_mysql,MYSQL_SET_CHARSET_NAME,"utf8");
		my_bool t=1;
		mysql_options(m_mysql,MYSQL_OPT_RECONNECT ,&t);
		if( m_mysql != mysql_real_connect( m_mysql,m_ip.c_str(),m_user.c_str(),m_pwd.c_str(),m_dbname.c_str(),atoi(port.c_str()),NULL,0) )
		{
			printf("Failed to connect to database: Error: %s\n",mysql_error(m_mysql));
			mysql_close(m_mysql);
			m_mysql = 0;
			return false;
		}
		return true;
	}
	void DBModuleMysql::Ping()
	{
		if( m_mysql )
		{
            int ret = mysql_ping(m_mysql);
			if( ret != 0 )
			{
				printf("mysql_ping error %d\n",ret);
			}
		}
	}
	void DBModuleMysql::Destroy( )
	{
		mysql_close(m_mysql);
		m_mysql = 0;
	}
	unsigned long DBModuleMysql::EscapeString( char* to, const char* from , unsigned long length )
	{
		return mysql_real_escape_string(m_mysql,to,from,length);
	}
	MYSQL_RES* DBModuleMysql::QueryUse( const char * sql,bool noret,my_ulonglong* effect )
	{
		if( ! m_mysql ) return 0;
		m_QueryCount ++;
		if( 0 != mysql_real_query( m_mysql,sql,(unsigned long)strlen(sql) ) )
		{
			printf( "On Query: %s, Error: \n", sql );
			printf( "%s\n", mysql_error(m_mysql) );
			return NULL;
		}
		if( effect )
		{
			*effect = mysql_affected_rows( m_mysql );
		}
		if( noret )
		{
			return NULL;
		}
		m_QueryResCount ++;
		return mysql_use_result(m_mysql);
	}
	MYSQL_RES* DBModuleMysql::QueryStore( const char * sql,bool noret,my_ulonglong* effect)
	{
		if( ! m_mysql ) return 0;
		if( effect ) *effect = 0;
		m_QueryCount ++;
		if( 0 != mysql_real_query( m_mysql,sql,(unsigned long)strlen(sql) ) )
		{
			printf( "On Query: %s, Error: \n", sql );
			printf( "%s\n", mysql_error(m_mysql) );
			return NULL;
		}
		if( effect )
		{
			*effect = mysql_affected_rows( m_mysql );
		}
		if( noret )
		{
			return NULL;
		}
		m_QueryResCount ++;
		return mysql_store_result(m_mysql);
	}
	void DBModuleMysql::FreeResult( MYSQL_RES* result)
	{
		m_QueryResFreeCount ++;
		mysql_free_result(result);
	}
	int DBModuleMysql::FindFieldPos(MYSQL_RES* result,const char* name)
	{
		if( ! m_mysql ) return 0;
		unsigned int num_fields;
		unsigned int i;
		MYSQL_FIELD *fields;

		num_fields = mysql_num_fields(result);
		fields = mysql_fetch_fields(result);
		int Pos = -1;
		for(i = 0; i < num_fields; i++)
		{
			if( strcmp( name,fields[i].name ) == 0 )
			{
				Pos = i;
				break;
			}
		}
		return Pos;
	}
	MYSQL_ROW DBModuleMysql::FetchRow(MYSQL_RES* result)
	{
		if( ! m_mysql ) return 0;
		MYSQL_ROW row;
		return row = mysql_fetch_row(result);
	}
	const char* DBModuleMysql::FetchField(MYSQL_ROW row,MYSQL_RES* result,const char* FieldName,int &length)
	{
		if( ! m_mysql ) return 0;
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(result);
		int Pos;
		if( -1 != (Pos = FindFieldPos(result,FieldName)) )
		{
			length = lengths[Pos];
			return row[Pos];
		}
		return NULL;
	}
}
