#pragma once
#import "c:\program files\common files\system\ado\msado20.tlb" no_namespace rename ("EOF","adoEOF")
namespace LEUD
{
	class DBModuleMssqlRecordSet
	{
	public:
		_RecordsetPtr m_pRecordSet;
		int			  m_iAffected;

	public:
		int GetFieldCount();
		_variant_t GetFieldValue(const char* strFieldName);
		void MoveNext();
		bool IsEof();
	};
	class DBModuleMssql
	{
		std::string     m_strConnectionString;
		_ConnectionPtr  m_pDBConnection;
	public:
		DBModuleMssql(void);
		virtual ~DBModuleMssql(void);

		bool Start(std::string connstr);
		void Stop();

		bool IsConn();
		void Query(const char* sql,DBModuleMssqlRecordSet* recordset);
	};
}