#pragma once
namespace sexy_db_mysql{
	class mem_db_row_imp;
	class mem_db_row{
		friend class mem_db_row_imp;
		mem_db_row_imp* _imp;
	public:
		const char* fetch_column(unsigned int column_idx,int* length);
	};
	class db_res;
	class mem_db_res_imp;
	class mem_db_res{
		friend class mem_db_res_imp;
		mem_db_res_imp* _imp;
	public:
		mem_db_res(){_imp=0;}
		unsigned int num_fields();
		long num_rows();
		bool get_data(const char** field_data,const char** row_data,unsigned int* num_fields,long* num_rows,size_t* fields_data_size,size_t* row_data_size,size_t* row_array_pos);
		void set_data_info(unsigned int num_fields,long num_rows,size_t fields_data_size,size_t row_data_size,size_t row_array_pos);
		void set_data_fields(size_t pos,const char* data,size_t len);
		void set_data_rows(size_t pos,const char* data,size_t len);
		unsigned int fetch_column_idx(const char* column_name);
		const char* fetch_column_name(unsigned int column_idx);
		mem_db_row* fetch_next_row();
		static mem_db_res create(db_res* fres);
		static mem_db_res create_from_mysqlres(void* fres);
		static void destroy(mem_db_res& dres);
	};
	class db_row_imp;
	class db_row{
		friend class db_row_imp;
		db_row_imp* _imp;
	public:
		const char* fetch_column(unsigned int column_idx,int* length);
	};
	class db_res_imp;
	class db_res{
		friend class db_res_imp;
		friend class mem_db_res;
		db_res_imp* _imp;
	public:
		unsigned int num_fields();
		long num_rows();
		unsigned int fetch_column_idx(const char* column_name);
		const char* fetch_column_name(unsigned int column_idx);
		db_row* fetch_next_row();
	};
	class db_mysql_imp;
	class db_mysql{
		friend class db_mysql_imp;
		db_mysql_imp* _imp;
	public:
		db_mysql();
		virtual ~db_mysql();
		void init(const char* ip,const char* port,const char* db,const char* user,const char* pwd);
		void dest();
		void query(const char* sql,void* arg,char need_affected_rows,char need_insert_id,char need_db_res);
		virtual void work_on_error(int errorid,const char* msg)=0;
		virtual void work_on_result(const char* sql,void* arg,int errorid,long* affected_rows,long* insert_id,sexy_db_mysql::db_res* res)=0;
	};
	int hex_string(char* to,const char* from,int len);
}
