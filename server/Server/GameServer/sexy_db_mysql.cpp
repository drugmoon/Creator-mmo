#include "sexy_db_mysql.h"
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#include <process.h>
#define WORK_THREAD_FUN_DEF(fun_name)				unsigned int __stdcall fun_name( void* arg )
#define WORK_THREAD_FUN_IMP(class_name,fun_name)	unsigned int __stdcall class_name::fun_name( void* arg )
#define WORK_THREAD_FUN_RET(return_value)			return return_value
#define WORK_THREAD_HANDLE							HANDLE
#define TRY __try
#define CATCH __except(UnhandledExceptionFilter(GetExceptionInformation()))
#else
#include <pthread.h>
#define WORK_THREAD_FUN_DEF(fun_name)				void* fun_name(void* arg)
#define WORK_THREAD_FUN_IMP(class_name,fun_name)	void* class_name::fun_name(void* arg)
#define WORK_THREAD_FUN_RET(return_value)			return (void*)return_value
#define WORK_THREAD_HANDLE							pthread_t
#define TRY try 
#define CATCH catch(...)
#define Sleep(ms) usleep(ms*1000)
#endif
#include <string>
#include <map>
#include <time.h>
#include "mysql.h"
#if defined(WIN32) || defined(WIN64)
#pragma comment(lib,"libmysql.lib")
#endif
namespace sexy_db_mysql{
	class CriticalSection{
	public:
		#if defined(WIN32) || defined(WIN64)
		CRITICAL_SECTION m_cs;
		CriticalSection(){InitializeCriticalSection(&m_cs);}
		~CriticalSection(){DeleteCriticalSection(&m_cs);}
		#else
		pthread_mutex_t m_cs;
		CriticalSection(){pthread_mutex_init(&m_cs,0);}
		~CriticalSection(){pthread_mutex_destroy(&m_cs);}
		#endif
		class Lock{
		public:
			CriticalSection& m_cs;
			#if defined(WIN32) || defined(WIN64)
			Lock(CriticalSection& cs):m_cs(cs){EnterCriticalSection( &m_cs.m_cs );}
			~Lock(){LeaveCriticalSection( &m_cs.m_cs );}
			#else
			Lock(CriticalSection& cs):m_cs(cs){pthread_mutex_lock( &m_cs.m_cs );}
			~Lock(){pthread_mutex_unlock( &m_cs.m_cs );}
			#endif
		};
	};
	class mem_db_row_imp{
	public:
		const char* _row;
		unsigned long* _lens;
		unsigned int _fields_num;
		void inst_bind(mem_db_row& inst){
			inst._imp=this;
		}
		void inst_unbind(mem_db_row& inst){
			inst._imp=0;
		}
		const char* fetch_column(unsigned int column_idx,int* length){
			*length=0;
			if(column_idx>=0&&column_idx<_fields_num){
				*length=_lens[column_idx];
				size_t* row_pos=(size_t*)_row;
				return _row+row_pos[column_idx];
			}
			return 0;
		}
	};
	const char* mem_db_row::fetch_column(unsigned int column_idx,int* length){
		*length=0;
		if(_imp)return _imp->fetch_column(column_idx,length);
		return 0;
	}
	const size_t fields_data_len=1024*1024;
	const size_t rows_data_len=1024*1024*9;
	class mem_db_res_imp{
	public:
		unsigned int _num_fields;
		long _num_rows;
		char _fields_data[fields_data_len];
		char _row_data[rows_data_len];
		long _cur_row;
		size_t _fields_data_size;
		size_t _row_data_size;
		size_t _row_array_pos;
		mem_db_row_imp _row_imp;
		mem_db_row _row_inst;
		mem_db_res_imp(){
			_cur_row=0;
		}
		void inst_bind(mem_db_res& inst){
			inst._imp=this;
		}
		void get_data(const char** field_data,const char** row_data,unsigned int* num_fields,long* num_rows,size_t* fields_data_size,size_t* row_data_size,size_t* row_array_pos){
			*field_data=_fields_data;
			*row_data=_row_data;
			*num_fields=_num_fields;
			*num_rows=_num_rows;
			*fields_data_size=_fields_data_size;
			*row_data_size=_row_data_size;
			*row_array_pos=_row_array_pos;
		}
		void set_data_info(unsigned int num_fields,long num_rows,size_t fields_data_size,size_t row_data_size,size_t row_array_pos){
			_num_fields=num_fields;
			_num_rows=num_rows;
			_fields_data_size=fields_data_size;
			_row_data_size=row_data_size;
			_row_array_pos=row_array_pos;
		}
		void set_data_fields(size_t pos,const char* data,size_t len){
			memcpy(_fields_data+pos,data,len);
		}
		void set_data_rows(size_t pos,const char* data,size_t len){
			memcpy(_row_data+pos,data,len);
		}
		unsigned int num_fields(){
			return _num_fields;
		}
		long num_rows(){
			return _num_rows;
		}
		unsigned int fetch_column_idx(const char* column_name){
			size_t* fields=(size_t*)_fields_data;
			for(unsigned int i=0;i<_num_fields;++i){
				if(0==strcmp(column_name,_fields_data+fields[i])){
					return i;
				}
			}
			return -1;
		}
		const char* fetch_column_name(unsigned int column_idx){
			size_t* fields=(size_t*)_fields_data;
			return _fields_data+fields[column_idx];
		}
		mem_db_row* fetch_next_row(){
			_row_imp.inst_unbind(_row_inst);
			size_t* row_p_array=(size_t*)(_fields_data+_row_array_pos);
			const char* row_data=_row_data+row_p_array[_cur_row];_cur_row++;
			_row_imp._lens=(unsigned long*)row_data;
			_row_imp._row=(const char*)(row_data+_num_fields*sizeof(unsigned long));
			_row_imp._fields_num=_num_fields;
			if(_row_imp._row&&_row_imp._lens){
				_row_imp.inst_bind(_row_inst);
				return &_row_inst;
			}
			return 0;
		}
	};
	unsigned int mem_db_res::num_fields(){
		if(_imp)return _imp->num_fields();
		return -1;
	}
	long mem_db_res::num_rows(){
		if(_imp)return _imp->num_rows();
		return -1;
	}
	bool mem_db_res::get_data(const char** field_data,const char** row_data,unsigned int* num_fields,long* num_rows,size_t* fields_data_size,size_t* row_data_size,size_t* row_array_pos){
		if(_imp){
			_imp->get_data(field_data,row_data,num_fields,num_rows,fields_data_size,row_data_size,row_array_pos);
			return true;
		}
		return false;
	}	
	void mem_db_res::set_data_info(unsigned int num_fields,long num_rows,size_t fields_data_size,size_t row_data_size,size_t row_array_pos){
		if(!_imp){
			mem_db_res_imp* imp=new mem_db_res_imp;imp->inst_bind(*this);
		}
		if(_imp){
			_imp->set_data_info(num_fields,num_rows,fields_data_size,row_data_size,row_array_pos);
		}
	}
	void mem_db_res::set_data_fields(size_t pos,const char* data,size_t len){
		if(_imp){
			_imp->set_data_fields(pos,data,len);
		}
	}
	void mem_db_res::set_data_rows(size_t pos,const char* data,size_t len){
		if(_imp){
			_imp->set_data_rows(pos,data,len);
		}
	}
	unsigned int mem_db_res::fetch_column_idx(const char* column_name){
		if(_imp)return _imp->fetch_column_idx(column_name);
		return -1;
	}
	const char* mem_db_res::fetch_column_name(unsigned int column_idx){
		if(_imp)return _imp->fetch_column_name(column_idx);
		return "";
	}
	mem_db_row* mem_db_res::fetch_next_row(){
		if(_imp)return _imp->fetch_next_row();
		return 0;
	}
	class db_row_imp{
	public:
		MYSQL_ROW _row;
		unsigned long* _lens;
		unsigned int _fields_num;
		void inst_bind(db_row& inst){
			inst._imp=this;
		}
		void inst_unbind(db_row& inst){
			inst._imp=0;
		}
		const char* fetch_column(unsigned int column_idx,int* length){
			*length=0;
			if(column_idx>=0&&column_idx<_fields_num){
				*length=_lens[column_idx];
				return _row[column_idx];
			}
			return 0;
		}
	};
	const char* db_row::fetch_column(unsigned int column_idx,int* length){
		*length=0;
		if(_imp)return _imp->fetch_column(column_idx,length);
		return 0;
	}
	class db_res_imp{
	public:
		friend class mem_db_res;
		MYSQL_RES* _res;
		db_row_imp _row_imp;
		db_row _row_inst;
		void inst_bind(db_res& inst){
			inst._imp=this;
		}
		unsigned int num_fields(){
			return mysql_num_fields(_res);
		}
		long num_rows(){
			return static_cast<long>(mysql_num_rows(_res));
		}
		unsigned int fetch_column_idx(const char* column_name){
			unsigned int fn=mysql_num_fields(_res);
			MYSQL_FIELD * fields=mysql_fetch_fields(_res);
			for(unsigned int i=0;i<fn;++i){
				if(0==strcmp(column_name,fields[i].name)){
					return i;
				}
			}
			return -1;
		}
		const char* fetch_column_name(unsigned int column_idx){
			MYSQL_FIELD * fields=mysql_fetch_fields(_res);
			return fields[column_idx].name;
		}
		db_row* fetch_next_row(){
			_row_imp.inst_unbind(_row_inst);
			_row_imp._row=mysql_fetch_row(_res);
			_row_imp._lens=mysql_fetch_lengths(_res);
			_row_imp._fields_num=mysql_num_fields(_res);
			if(_row_imp._row&&_row_imp._lens){
				_row_imp.inst_bind(_row_inst);
				return &_row_inst;
			}
			return 0;
		}
	};
	unsigned int db_res::num_fields(){
		if(_imp)return _imp->num_fields();
		return -1;
	}
	long db_res::num_rows(){
		if(_imp)return _imp->num_rows();
		return -1;
	}
	unsigned int db_res::fetch_column_idx(const char* column_name){
		if(_imp)return _imp->fetch_column_idx(column_name);
		return -1;
	}
	const char* db_res::fetch_column_name(unsigned int column_idx){
		if(_imp)return _imp->fetch_column_name(column_idx);
		return "";
	}
	db_row* db_res::fetch_next_row(){
		if(_imp)return _imp->fetch_next_row();
		return 0;
	}
	class db_mysql_event{
	public:
		db_mysql_event* _next;
		const char* _sql;
		void * _arg;
		char _need_affected_rows;
		char _need_insert_id;
		char _need_db_res;
	};
	class db_mysql_imp{
		friend class db_mysql;
		db_mysql* _inst;
		volatile int _work_thread_stop_event;
		WORK_THREAD_HANDLE _work_thread_handle;
		std::string _ip;
		std::string _port;
		std::string _db;
		std::string _user;
		std::string _pwd;
		time_t _ping_time;
		MYSQL* _mysql;
		CriticalSection _event_cs;
		db_mysql_event* _event_queue;
		db_mysql_imp(db_mysql* inst){
			_inst=inst;_ping_time=0;_mysql=0;_event_queue=0;_work_thread_stop_event=0;
		}
		~db_mysql_imp(){
			_inst=0;
		}
		void init(const char* ip,const char* port,const char* db,const char* user,const char* pwd){
			_ip=ip;_port=port;_db=db;_user=user;_pwd=pwd;
#if defined(WIN32) || defined(WIN64)
			unsigned int tid;_work_thread_handle = (HANDLE)_beginthreadex(NULL, 0, work_thread_fun, this, 0, &tid );
#else
			pthread_create(&_work_thread_handle,NULL,work_thread_fun,this);
#endif
		}
		void dest(){
			_work_thread_stop_event=1;
#if defined(WIN32) || defined(WIN64)
			WaitForSingleObject(_work_thread_handle,INFINITE);
#else
			pthread_join(_work_thread_handle,0);
#endif
		}
		void query(const char* sql,void* arg,char need_affected_rows,char need_insert_id,char need_db_res){
			if(_work_thread_stop_event!=0){_inst->work_on_result(sql,arg,0,0,0,0);return;}
			db_mysql_event* e=new db_mysql_event;memset(e,0,sizeof(db_mysql_event));
			e->_sql=sql;e->_arg=arg;e->_need_affected_rows=need_affected_rows;e->_need_insert_id=need_insert_id;e->_need_db_res=need_db_res;
			CriticalSection::Lock lock(_event_cs);
			e->_next=_event_queue;_event_queue=e;
		}
		void start(){
			_mysql=mysql_init(0);
			if(_mysql){
				mysql_options(_mysql,MYSQL_SET_CHARSET_NAME,"utf8");my_bool t=1;mysql_options(_mysql,MYSQL_OPT_RECONNECT ,&t);
				if( _mysql != mysql_real_connect(_mysql,_ip.c_str(),_user.c_str(),_pwd.c_str(),_db.c_str(),atoi(_port.c_str()),NULL,0) ){
					_inst->work_on_error(-1,mysql_error(_mysql));mysql_close(_mysql);_mysql=0;
				}				
			}
		}
		void stop(){
			if(_mysql){mysql_close(_mysql);_mysql=0;}
		}
		void process(){
			if(!_mysql)return;
			db_mysql_event* queue=0;
			{
				CriticalSection::Lock lock(_event_cs);
				while(_event_queue){
					db_mysql_event* t=_event_queue;_event_queue=_event_queue->_next;
					t->_next=queue;queue=t;
				}
			}
			long affected_rows;long insert_id;db_res res;db_res_imp res_imp;res_imp.inst_bind(res);
			while(queue){
				db_mysql_event* e=queue;queue=queue->_next;
				if(0!=mysql_real_query(_mysql,e->_sql,(unsigned long)strlen(e->_sql))){
					_inst->work_on_error(-1,mysql_error(_mysql));
					_inst->work_on_result(e->_sql,e->_arg,0,0,0,0);
					continue;
				}
				affected_rows=0;
				if(e->_need_affected_rows){
					affected_rows=static_cast<long>(mysql_affected_rows(_mysql));
				}
				insert_id=0;
				if(e->_need_insert_id){
					insert_id=static_cast<long>(mysql_insert_id(_mysql));
				}
				res_imp._res=0;
				if(e->_need_db_res){
					res_imp._res=mysql_store_result(_mysql);
				}
				_inst->work_on_result(e->_sql,e->_arg,0,(e->_need_affected_rows?&affected_rows:0),(e->_need_insert_id?&insert_id:0),(e->_need_db_res?&res:0));
				if(res_imp._res){
					mysql_free_result(res_imp._res);res_imp._res=0;
				}
				delete e;
			}
		}
		void check_ping(){
			if(time(0)>_ping_time+30){
				if(_mysql){mysql_ping(_mysql);}
				_ping_time=time(0);
			}
		}
		static WORK_THREAD_FUN_DEF(work_thread_fun);
	};
	WORK_THREAD_FUN_IMP(db_mysql_imp,work_thread_fun){
		db_mysql_imp* This=(db_mysql_imp*)arg;
		This->start();
		while(This->_work_thread_stop_event==0){
			TRY{This->process();}CATCH{}
			This->check_ping();
			Sleep(100);
		}
		TRY{This->process();}CATCH{}
		TRY{This->process();}CATCH{}
		This->stop();
		WORK_THREAD_FUN_RET(0);
	}
	db_mysql::db_mysql(){
		_imp=0;_imp=new db_mysql_imp(this);
	}
	db_mysql::~db_mysql(){
		delete _imp;_imp=0;
	}
	void db_mysql::init(const char* ip,const char* port,const char* db,const char* user,const char* pwd){
		_imp->init(ip,port,db,user,pwd);
	}
	void db_mysql::dest(){
		_imp->dest();
	}
	void db_mysql::query(const char* sql,void* arg,char need_effect_rows_num,char need_insert_new_id,char need_db_res){
		_imp->query(sql,arg,need_effect_rows_num,need_insert_new_id,need_db_res);
	}
	static char HEX_STRING_TABLE[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	int hex_string(char* to,const char* from,int len){
		int ret=0;
		for(int i=0;i<len;i++){
			char f=from[i];
			*to=HEX_STRING_TABLE[(f&0xF0)>>4];
			to++;ret++;
			*to=HEX_STRING_TABLE[f&0xF];
			to++;ret++;
		}
		*to=0;
		return ret;
		//return mysql_hex_string(to,from,len);
	}
	mem_db_res mem_db_res::create_from_mysqlres(void* fres){
		db_res res;db_res_imp res_imp;res_imp.inst_bind(res);res_imp._res=(MYSQL_RES*)fres;
		return mem_db_res::create(&res);
	}
	mem_db_res mem_db_res::create(db_res* fres){
		mem_db_res inst;memset(&inst,0,sizeof(mem_db_res));
		mem_db_res_imp* imp=new mem_db_res_imp;
		imp->_num_fields=fres->num_fields();
		//fields data
		size_t* fields_p_array=(size_t*)imp->_fields_data;
		char* fields_data_begin=&imp->_fields_data[imp->_num_fields*sizeof(size_t)];
		char* fields_data_end=&imp->_fields_data[fields_data_len-1];
		MYSQL_FIELD * src_fields=mysql_fetch_fields(fres->_imp->_res);
		for(unsigned int i=0;i<imp->_num_fields;i++){
			fields_p_array[i]=fields_data_begin-imp->_fields_data;
			if(fields_data_begin+strlen(src_fields[i].name)+1 > fields_data_end){ delete imp;return inst; }
			strcpy(fields_data_begin,src_fields[i].name);fields_data_begin+=strlen(src_fields[i].name);*fields_data_begin=0;fields_data_begin+=1;
		}
		//rows data
		imp->_row_array_pos=fields_data_begin-imp->_fields_data;
		size_t* rows_data_p_array=(size_t*)fields_data_begin;
		char* rows_data_begin=imp->_row_data;
		char* rows_data_end=&imp->_row_data[rows_data_len-1];
		const char** src_row;long i=0;
		while(src_row=(const char**)mysql_fetch_row(fres->_imp->_res)){
			if(fields_data_begin+sizeof(size_t) > fields_data_end){ delete imp;return inst; }
			rows_data_p_array[i++]=rows_data_begin-imp->_row_data;fields_data_begin+=sizeof(size_t);
			unsigned long* src_lens=mysql_fetch_lengths(fres->_imp->_res);

			if(rows_data_begin+imp->_num_fields*sizeof(unsigned long) > rows_data_end){ delete imp;return inst; }
			memcpy(rows_data_begin,src_lens,imp->_num_fields*sizeof(unsigned long));rows_data_begin+=imp->_num_fields*sizeof(unsigned long);

			const char* rows_data_begin_pos=rows_data_begin;
			if(rows_data_begin+imp->_num_fields*sizeof(size_t) > rows_data_end){ delete imp;return inst; }
			size_t* row_p_array=(size_t*)rows_data_begin;rows_data_begin+=imp->_num_fields*sizeof(size_t);

			for(unsigned int j=0;j<imp->_num_fields;j++){
				if(src_row[j]){
					row_p_array[j]=rows_data_begin-rows_data_begin_pos;
					if(rows_data_begin+src_lens[j]+1 > rows_data_end){ delete imp;return inst; }
					memcpy(rows_data_begin,src_row[j],src_lens[j]);rows_data_begin+=src_lens[j];*rows_data_begin=0;rows_data_begin+=1;
				}else{row_p_array[j]=0;}
			}
		}
		imp->_num_rows=i;
		imp->_fields_data_size = fields_data_begin-imp->_fields_data;
		imp->_row_data_size=rows_data_begin-imp->_row_data;
		imp->_cur_row=0;
		imp->inst_bind(inst);
		return inst;
	}
	void mem_db_res::destroy(mem_db_res& dres){
		delete dres._imp;dres._imp=0;
	}
}
