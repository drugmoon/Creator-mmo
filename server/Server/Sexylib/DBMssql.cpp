#include "StdAfx.h"
#include "leudgrid/dbmssql.h"
namespace LEUD
{
	DBModuleMssql::DBModuleMssql(void)
	{
	}
	DBModuleMssql::~DBModuleMssql(void)
	{
	}
	bool DBModuleMssql::Start(std::string connstr)
	{
		m_strConnectionString = connstr;
		try
		{
			HRESULT hr;
			hr = m_pDBConnection.CreateInstance(__uuidof(Connection));
			if( FAILED( hr ) )
			{
				_com_issue_error(hr);
			}
			hr = m_pDBConnection->Open(_bstr_t(m_strConnectionString.c_str()),"","",adConnectUnspecified);
			if( FAILED( hr ) )
			{
				_com_issue_error(hr);
			}
		}
		catch(_com_error& e)
		{
			printf("%s %d\n",e.ErrorMessage(),e.WCode());
			return false;
		}
		return true;
	}
	bool DBModuleMssql::IsConn()
	{
		if( m_pDBConnection != 0 && m_pDBConnection->State == adStateOpen )
		{
			return true;
		}
		return false;
	}
	void DBModuleMssql::Stop()
	{
		if( m_pDBConnection != 0 && m_pDBConnection->State != adStateClosed )
		{
			m_pDBConnection->Close();
		}
	}
	void DBModuleMssql::Query(const char* sql,DBModuleMssqlRecordSet* recordset)
	{
		try
		{
			if( recordset )
			{
				_variant_t var;
				recordset->m_pRecordSet = m_pDBConnection->Execute(_bstr_t(sql),&var,adCmdText);
				recordset->m_iAffected = int(var);
			}
			else
			{
				m_pDBConnection->Execute(_bstr_t(sql),0,adCmdText);
			}
		}
		catch(_com_error& e)
		{
			printf("%s\n",e.ErrorMessage());
		}
	}
	int DBModuleMssqlRecordSet::GetFieldCount()
	{
		if( m_pRecordSet ) return m_pRecordSet->Fields->Count;
		return 0;
	}
	_variant_t DBModuleMssqlRecordSet::GetFieldValue(const char* strFieldName)
	{
		_variant_t vtRet;
		if( !IsEof() ) vtRet = m_pRecordSet->Fields->GetItem(strFieldName)->Value;
		return vtRet;
	}
	void DBModuleMssqlRecordSet::MoveNext()
	{
		if( !IsEof() ) m_pRecordSet->MoveNext();
	}
	bool DBModuleMssqlRecordSet::IsEof()
	{
		if( (m_pRecordSet.operator bool()) && (!m_pRecordSet->adoEOF) ) return false;
		return true;
	}
}